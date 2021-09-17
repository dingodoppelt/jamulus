/******************************************************************************\
 * Copyright (c) 2021
 *
 * Author(s):
 *  dtinth
 *
 ******************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 \******************************************************************************/

#include "stereomixserver.h"

CStereoMixServer::CStereoMixServer ( CServer* pServer, int iPort ) :
    QObject ( pServer ),
    pServer ( pServer ),
    iPort ( iPort ),
    pTransportServer ( new QTcpServer ( this ) )
{
    connect ( pTransportServer, &QTcpServer::newConnection, this, &CStereoMixServer::OnNewConnection );
    connect ( pServer, &CServer::StreamFrame, this, &CStereoMixServer::OnStreamFrame );
}

CStereoMixServer::~CStereoMixServer()
{
    if ( pTransportServer->isListening() )
    {
        qInfo() << "- stopping stereo mix server";
        pTransportServer->close();
    }
}

void CStereoMixServer::Start()
{
    if ( iPort < 0 )
    {
        return;
    }
    if ( pTransportServer->listen ( QHostAddress ( "127.0.0.1" ), iPort ) )
    {
        qInfo() << "- stereo mix server started on port" << pTransportServer->serverPort();
    }
    else
    {
        qInfo() << "- unable to start stereo mix server:" << pTransportServer->errorString();
    }
}

void CStereoMixServer::OnNewConnection()
{
    QTcpSocket* pSocket = pTransportServer->nextPendingConnection();
    if ( !pSocket )
    {
        return;
    }

    qInfo() << "- sending stereo mix to:" << pSocket->peerAddress().toString();
    vecClients.append ( pSocket );

    connect ( pSocket, &QTcpSocket::disconnected, [this, pSocket]() {
        qInfo() << "- finish sending stereo mix to:" << pSocket->peerAddress().toString();
        vecClients.removeAll ( pSocket );
        pSocket->deleteLater();
    } );
}

void CStereoMixServer::OnStreamFrame ( const int iServerFrameSizeSamples, const CVector<int16_t>& data )
{
    for ( auto socket : vecClients )
    {
        socket->write ( reinterpret_cast<const char*> ( &data[0] ), sizeof ( int16_t ) * ( 2 * iServerFrameSizeSamples ) );
    }
}
