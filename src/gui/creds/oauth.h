/*
 * Copyright (C) by Olivier Goffart <ogoffart@woboq.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#pragma once
#include <QPointer>
#include <QTcpServer>
#include <QUrl>
#include "accountfwd.h"

namespace OCC {

/**
 * Job that do the authorization grant and fetch the access token
 *
 * Normal workflow:
 *
 *   --> start()
 *       |
 *       +----> fetchWellKnown() query the ".well-known/openid-configuration" endpoint
 *       |
 *       +----> openBrowser() open the browser to the login page after fetchWellKnown finished.
 *       |                    Then the browser will redirect to http://localhost:xxx
 *       |
 *       +----> _server starts listening on a TCP port waiting for an HTTP request with a 'code'
 *                |
 *                v
 *             request the access_token and the refresh_token via 'apps/oauth2/api/v1/token'
 *                |
 *                +-> Request the user_id is not present
 *                |     |
 *                v     v
 *              finalize(...): emit result(...)
 *
 */
class OAuth : public QObject
{
    Q_OBJECT
public:
    OAuth(Account *account, QObject *parent)
        : QObject(parent)
        , _account(account)
    {
    }
    ~OAuth();

    enum Result { NotSupported,
        LoggedIn,
        Error };
    Q_ENUM(Result);
    void start();
    void openBrowser();
    QUrl authorisationLink() const;
    /**
     * Call the callback when the call to the well-known endpoint finishes.
     * (or immediatly if it is ready)
     * The callback will not be called if this object gets destroyed
     */
    void authorisationLinkAsync(std::function<void(const QUrl&)> callback) const;

signals:
    /**
     * The state has changed.
     * when logged in, token has the value of the token.
     */
    void result(OAuth::Result result, const QString &user = QString(), const QString &token = QString(), const QString &refreshToken = QString());

    /**
     * emitted when the call to the well-known endpoint is finished
     */
    void authorisationLinkChanged(const QUrl &);

private:

    void fetchWellKnown();
    void finalize(QPointer<QTcpSocket> socket, const QString &accessToken,
                  const QString &refreshToken, const QString &userId, const QUrl &messageUrl);

    Account* _account;
    QTcpServer _server;
    bool _wellKnownFinished = false;
    QUrl _authEndpoint;
    QUrl _tokenEndpoint;
    QByteArray _pkceCodeVerifier;

public:
    QString _expectedUser;
};


} // namespace OCC
