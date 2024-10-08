/*
 * Copyright (C) 2014-2015 Canonical, Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Stefano Verzegnassi <stefano92.100@gmail.com>
 */

#include "pdfitem.h"

PdfItem::PdfItem(const QString &url, const QSize &size, const QVariantList &links) :
    m_width(size.width())
    , m_height(size.height())
    , m_url(url)
    ,m_pageLinks(links)
{
}

int PdfItem::width() const
{
    return m_width;
}

int PdfItem::height() const
{
    return m_height;
}

QString PdfItem::url() const
{
    return m_url;
}

QVariantList PdfItem::links() const
{
    return m_pageLinks;
}
