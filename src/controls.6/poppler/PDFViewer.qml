import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.mauikit.controls as Maui
import org.mauikit.documents as Poppler

Maui.Page
{
    id: control

    property bool fitWidth: false
    property int currentPage : _listView.currentIndex
    property alias currentItem :_listView.currentItem
    property alias orientation : _listView.orientation
    property alias path : poppler.path
    property color searchHighlightColor: Qt.rgba(1, 1, .2, .4)

    headBar.visible: true
    footBar.visible: true
    title:  poppler.title
    padding: 0

    Maui.InputDialog
    {
        id: _passwordDialog

        title: i18n("Document Locked")
        message: i18n("Please enter your password to unlock and open the file.")
        textEntry.echoMode: TextInput.Password
        onFinished: poppler.unlock(text, text)
    }

    footerColumn: Maui.ToolBar
    {
        width: parent.width
        middleContent: Maui.SearchField
        {
            Layout.fillWidth: true
            Layout.maximumWidth: 500
            Layout.alignment: Qt.AlignHCenter

            onAccepted:
            {
                console.log("SEARCH FOR ", text)
                search(text)
            }

            actions: [
                Action
                {
                    text: i18n("Case sensitive")
                    icon.name: "format-text-uppercase"
                    checked: searchSensitivity === Qt.CaseInsensitive
                    onTriggered:
                    {
                        if(searchSensitivity === Qt.CaseInsensitive)
                        {
                            searchSensitivity = Qt.CaseSensitive
                        }else
                        {
                            searchSensitivity = Qt.CaseInsensitive
                        }
                    }
                }
            ]
        }
    }

    footBar.middleContent: Maui.ToolActions
    {
        Layout.alignment: Qt.AlignHCenter
        expanded: true
        autoExclusive: false
        checkable: false

        Action
        {
            enabled: _listView.currentIndex > 0
            icon.name:  _listView.orientation === ListView.Horizontal ? "go-previous" : "go-up"
            onTriggered:
            {
                if( _listView.currentIndex > 0)
                    _listView.currentIndex = _listView.currentIndex - 1
            }
        }

        Action
        {
            text:  _listView.currentIndex + 1 +" / "+ poppler.pages
        }

        Action
        {
            enabled: _listView.currentIndex +1 < poppler.pages
            icon.name:  _listView.orientation === ListView.Horizontal ? "go-next" : "go-down"
            onTriggered:
            {
                if( _listView.currentIndex +1 < poppler.pages)
                    _listView.currentIndex = _listView.currentIndex + 1
            }
        }
    }

    Maui.ListBrowser
    {
        id: _listView
        anchors.fill: parent
        model: Poppler.Document
        {
            id: poppler

            property bool isLoading: true

            onPagesLoaded:
            {
                isLoading = false;
            }

            onDocumentLocked: _passwordDialog.open()
        }

        orientation: ListView.Vertical
        snapMode: ListView.SnapOneItem
        //        cacheBuffer: control.fitWidth ? poppler.providersNumber *  : height * poppler.providersNumber

        flickable.onMovementEnded:
        {
            var index = indexAt(_listView.contentX, _listView.contentY)
            currentIndex = index
        }

        delegate: Maui.ImageViewer
        {
            id: pageImg
            asynchronous: true
            width: ListView.view.width
            height: ListView.view.height

            cache: false
            //                source: "image://poppler" + (index % poppler.providersNumber) + "/page/" + _listView.currentPage;
            //                source: "image://poppler" + (index % poppler.providersNumber) + "/page/" + index;
            source: model.url
            //                                source: "image://poppler/page/" + _listView.currentPage;
            sourceSize.width: model.width * (contentWidth/model.width)
            sourceSize.height: model.height * (contentHeight/model.height)
            //                    sourceSize.height: 2000
            //                    imageWidth: 1000
            //                    imageHeight: 1000
            // fillMode: Image.Pad

            //                onSourceChanged: console.log(source)

            readonly property var links : model.links
            Repeater
            {
                model: links
                delegate: MouseArea
                {
                    x: Math.round(modelData.rect.x * parent.width)
                    y: Math.round(modelData.rect.y * parent.height)
                    width: Math.round(modelData.rect.width * parent.width)
                    height: Math.round(modelData.rect.height * parent.height)

                    cursorShape: Qt.PointingHandCursor
                    onClicked: __goTo(modelData.destination)
                }
            }

            Rectangle
            {
                visible: __currentSearchResult.page === index
                color: control.searchHighlightColor
                x: Math.round(__currentSearchResult.rect.x * parent.width)
                y: Math.round(__currentSearchResult.rect.y * parent.height)
                width: Math.round(__currentSearchResult.rect.width * parent.width)
                height: Math.round(__currentSearchResult.rect.height * parent.height)
            }
        }
    }

    Maui.Holder
    {
        visible: !poppler.isValid
        anchors.fill: parent
        emoji: poppler.isLocked ? "qrc:/img_assets/assets/lock.svg" : "qrc:/img_assets/assets/alarm.svg"
        title: poppler.isLocked ? i18n("Locked") : i18n("Error")
        body: poppler.isLocked ? i18n("This document is password protected.") : i18n("There has been an error loading this document.")

        actions: Action
        {
            enabled: poppler.isLocked
            text: i18n("UnLock")
            onTriggered: _passwordDialog.open()
        }
    }

    function open(filePath)
    {
        poppler.path = filePath
    }

    function __goTo (destination)
    {
        _listView.flickable.positionViewAtIndex(destination.page, ListView.Beginning)
        var pageHeight = poppler.pages[destination.page].size.height * zoom
        var scroll = Math.round(destination.top * pageHeight)
        _listView.contentY += scroll
    }

    function search(text)
    {
        if (!poppler.isValid)
            return

        console.log("2 SEARCH FOR ", text)

        if (text.length === 0)
        {
            __currentSearchTerm = ''
            __currentSearchResultIndex = -1
            __currentSearchResults = []
        } else if (text === __currentSearchTerm)
        {
            if (__currentSearchResultIndex < __currentSearchResults.length - 1)
            {
                __currentSearchResultIndex++
                __scrollTo(__currentSearchResult)
            } else
            {
                var page = __currentSearchResult.page
                __currentSearchResultIndex = -1
                __currentSearchResults = []
                if (page < _listView.count - 1)
                {
                    __search(page + 1, __currentSearchTerm)
                } else
                {
                    control.searchRestartedFromTheBeginning()
                    __search(0, __currentSearchTerm)
                }
            }
        } else
        {
            __currentSearchTerm = text
            __currentSearchResultIndex = -1
            __currentSearchResults = []
            __search(currentPage, text)
        }
    }

    signal searchNotFound
    signal searchRestartedFromTheBeginning

    property int searchSensitivity: Qt.CaseInsensitive
    property string __currentSearchTerm
    property int __currentSearchResultIndex: -1
    property var __currentSearchResults
    property var __currentSearchResult: __currentSearchResultIndex > -1 ? __currentSearchResults[__currentSearchResultIndex] : { page: -1, rect: Qt.rect(0,0,0,0) }

    function __search(startPage, text)
    {
        if (startPage >= _listView.count)
            throw new Error('Start page index is larger than number of pages in document')

        function resultFound(page, result)
        {
            var searchResults = []
            for (var i = 0; i < result.length; ++i)
            {
                searchResults.push({ page: page, rect: result[i] })
            }
            __currentSearchResults = searchResults
            __currentSearchResultIndex = 0
            __scrollTo(__currentSearchResult)
        }

        var found = false
        for (var page = startPage; page < _listView.count; ++page)
        {
            var result = poppler.search(page, text, searchSensitivity)

            if (result.length > 0)
            {
                found = true
                resultFound(page, result)
                break
            }
        }

        if (!found)
        {
            for (page = 0; page < startPage; ++page)
            {
                result = poppler.search(page, text, searchSensitivity)

                if (result.length > 0)
                {
                    found = true
                    control.searchRestartedFromTheBeginning()
                    resultFound(page, result)
                    break
                }
            }
        }

        if (!found)
        {
            control.searchNotFound()
        }
    }

    function __scrollTo(destination)
    {
        if (destination.page !== currentPage)
        {
            _listView.flickable.positionViewAtIndex(destination.page, ListView.Beginning)
        }

        var i = _listView.flickable.itemAt(_listView.width / 2, _listView.contentY + _listView.height / 2)
        if (i === null)
            i = _listView.flickable.itemAt(_listView.width / 2, _listView.contentY + _listView.height / 2 + _listView.spacing)

        // var pageHeight = poppler.pages[destination.page].size.height * zoom
        var pageHeight = control.height
        var pageY = i.y - _listView.contentY

        var bottomDistance = _listView.height - (pageY + Math.round(destination.rect.bottom * pageHeight))
        var topDistance = pageY + Math.round(destination.rect.top * pageHeight)
        if (bottomDistance < 0)
        {
            // The found term is lower than the bottom of viewport
            _listView.contentY -= bottomDistance - _listView.spacing
        } else if (topDistance < 0)
        {
            _listView.contentY += topDistance - _listView.spacing
        }
    }

}
