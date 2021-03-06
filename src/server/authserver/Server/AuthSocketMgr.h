/*
 * Copyright (C) 2015 TinyCore <http://www.tinycore.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AuthSocketMgr_h__
#define AuthSocketMgr_h__

#include "SocketMgr.h"
#include "AuthSession.h"

void OnSocketAccept(tcp::socket&& sock);

class AuthSocketMgr : public SocketMgr<AuthSession>
{
    typedef SocketMgr<AuthSession> BaseSocketMgr;

public:
    static AuthSocketMgr& Instance()
    {
        static AuthSocketMgr instance;
        return instance;
    }

    bool StartNetwork(boost::asio::io_service& service, std::string const& bindIp, uint16 port) override
    {
        if (!BaseSocketMgr::StartNetwork(service, bindIp, port))
            return false;

        _acceptor->AsyncAcceptManaged(&OnSocketAccept);
        return true;
    }

protected:
    NetworkThread<AuthSession>* CreateThreads() const override
    {
        return new NetworkThread<AuthSession>[1];
    }
};

#define sAuthSocketMgr AuthSocketMgr::Instance()

void OnSocketAccept(tcp::socket&& sock)
{
    sAuthSocketMgr.OnSocketOpen(std::forward<tcp::socket>(sock));
}


#endif // AuthSocketMgr_h__
