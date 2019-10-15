/*
 * Copyright (C) 2015 by Jeroen Hoek
 * Copyright (C) 2015 by Olivier Goffart <ogoffart@owncloud.com>
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

#include "wizard/owncloudconnectionmethoddialog.h"
#include <QUrl>

namespace OCC {

OwncloudConnectionMethodDialog::OwncloudConnectionMethodDialog(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint)
    , ui(new Ui::OwncloudConnectionMethodDialog)
{
    ui->setupUi(this);

    connect(ui->btnNoTLS, &QAbstractButton::clicked, this, &OwncloudConnectionMethodDialog::returnNoTLS);
    connect(ui->btnClientSideTLS, &QAbstractButton::clicked, this, &OwncloudConnectionMethodDialog::returnClientSideTLS);
    connect(ui->btnBack, &QAbstractButton::clicked, this, &OwncloudConnectionMethodDialog::returnBack);
}

void OwncloudConnectionMethodDialog::setUrl(const QUrl &url)
{
    ui->label->setText(tr("&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Falha na conexão com o endereço do servidor seguro &lt;em&gt;%1&lt;/em&gt;. Como você deseja prosseguir?&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;").arg(url.toDisplayString().toHtmlEscaped()));
}


void OwncloudConnectionMethodDialog::returnNoTLS()
{
    done(No_TLS);
}

void OwncloudConnectionMethodDialog::returnClientSideTLS()
{
    done(Client_Side_TLS);
}

void OwncloudConnectionMethodDialog::returnBack()
{
    done(Back);
}

OwncloudConnectionMethodDialog::~OwncloudConnectionMethodDialog()
{
    delete ui;
}
}
