/*
*  This file is part of aasdk library project.
*  Copyright (C) 2018 f1x.studio (Michal Szwaj)
*
*  aasdk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 3 of the License, or
*  (at your option) any later version.

*  aasdk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with aasdk. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <list>
#include <f1x/aasdk/TCP/ITCPEndpoint.hpp>
#include <f1x/aasdk/Transport/DataSink.hpp>
#include <f1x/aasdk/Transport/ITransport.hpp>

namespace f1x
{
namespace aasdk
{
namespace transport
{

class TCPTransport: public ITransport, public std::enable_shared_from_this<TCPTransport>, boost::noncopyable
{
public:
    TCPTransport(boost::asio::io_service& ioService, tcp::ITCPEndpoint::Pointer tcpEndpoint);

    void receive(size_t size, ReceivePromise::Pointer promise) override;
    void send(common::Data data, SendPromise::Pointer promise) override;
    void stop() override;

private:
    typedef std::list<std::pair<size_t, ReceivePromise::Pointer>> ReceiveQueue;
    typedef std::list<std::pair<common::Data, SendPromise::Pointer>> SendQueue;

    using std::enable_shared_from_this<TCPTransport>::shared_from_this;
    void distributeReceivedData();
    void receiveHandler(size_t bytesTransferred);
    void receiveFailureHandler(const aasdk::error::Error& e);

    void doSend();
    void sendHandler(size_t bytesTransferred, SendQueue::iterator queueElement);
    void sendFailureHandler(const aasdk::error::Error& e, SendQueue::iterator queueElement);

    boost::asio::io_service& ioService_;
    tcp::ITCPEndpoint::Pointer tcpEndpoint_;

    boost::asio::io_service::strand receiveStrand_;
    ReceiveQueue receiveQueue_;
    DataSink tcpReceivedDataSink_;

    boost::asio::io_service::strand sendStrand_;
    SendQueue sendQueue_;
};

}
}
}
