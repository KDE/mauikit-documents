/*
 * Copyright (C) 2015 Dan Leinir Turthra Jensen <admin@leinir.dk>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef ACBFDOCUMENT_H
#define ACBFDOCUMENT_H

#include <memory>

#include <QObject>
#include "AcbfMetadata.h"
#include "AcbfBody.h"
#include "AcbfReferences.h"
#include "AcbfData.h"
#include "AcbfStyleSheet.h"
/**
 * \brief Class that handles all of the ACBF document.
 * 
 * ACBF documents are made up of several subsections:
 * 
 * - Metadata, which in turn holds book, publishing and document info.
 * - Body, which holds the pages and their frame and text definitions.
 * - Data, which holds embedded data like images and fonts.
 * - References, a section which holds notes and references that can be pointed
 * at from the text. Not supported currently.
 * - Stylesheet, which is a css stylesheet to inform how overlaid translations
 * should be rendered. Not supported currently.
 * 
 * Of these, only Body and Metadata are necessary for a proper ACBF file.
 * 
 */
namespace AdvancedComicBookFormat
{
class  Document : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AdvancedComicBookFormat::Metadata *metaData READ metaData NOTIFY metaDataChanged)
    Q_PROPERTY(AdvancedComicBookFormat::Body *body READ body NOTIFY bodyChanged)
    Q_PROPERTY(AdvancedComicBookFormat::References *references READ references NOTIFY referencesChanged)
    Q_PROPERTY(AdvancedComicBookFormat::Data *data READ data NOTIFY dataChanged)
    Q_PROPERTY(AdvancedComicBookFormat::StyleSheet *styleSheet READ styleSheet NOTIFY stylesheetChanged)
public:
    explicit Document(QObject* parent = nullptr);
    ~Document() override;

    /**
     * \brief write the whole document to an ACBF xml.
     */
    QString toXml();
    /**
     * \brief load an ACBF file from XML.
     * @return True if the xmlReader encountered no errors.
     */
    bool fromXml(QString xmlDocument);

    /**
     * @returns The metadata object.
     */
    Metadata* metaData() const;
    /**
     * @brief fires when the metadata is changed.
     */
    Q_SIGNAL void metaDataChanged();

    /**
     * @returns the Body object.
     */
    Body* body() const;
    /**
     * @return fires when the body changes. Not used.
     */
    Q_SIGNAL void bodyChanged();
    
    /**
     * @brief The reference section.
     * 
     * @return a References object with the references.
     */
    References* references() const;
    /**
     * Fired when the references change. Not used (track the references inside the References object itself).
     */
    Q_SIGNAL void referencesChanged();

    /**
     * @returns the Data object.
     */
    Data* data() const;
    /**
     * Fired when the data changes. Not used (track the binary data inside the Data object itself)
     */
    Q_SIGNAL void dataChanged();

    /**
     * @brief The style section, which contains a css stylesheet.
     * 
     * @return A StyleSheet object with the css.
     */
    StyleSheet* styleSheet() const;
    /**
     * Fired when the stylesheet changes.
     * Not used (track the stylesheet information inside the stylesheet object itself)
     */
    Q_SIGNAL void stylesheetChanged();

    /**
     * Find some child object by their string ID
     * @see AdvancedComicBookFormat::Reference
     * @see AdvancedComicBookFormat::Binary
     */
    QObject* objectByID(const QString& id) const;
private:
    class Private;
    std::unique_ptr<Private> d;
};
}

#endif//ACBFDOCUMENT_H
