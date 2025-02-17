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
    
    /**
     * @brief Whether to enable the lasso selection, to select multiple items.
     * By default this is set to `false`.
     * @see itemsSelected
     */
    property bool enableLassoSelection : true
    
    /**
     * @brief Emitted when an empty space of the background area has been clicked.
     * @param mouse Object with information about the click event.
     */
    signal areaClicked(var mouse)
    
    /**
     * @brief Emitted when an empty space of the area area background has been right clicked.
     */
    signal areaRightClicked()
    
    signal areaSelected(var rect)

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
                    checkable: true
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
readonly property int page: index
            cache: false
            //                source: "image://poppler" + (index % poppler.providersNumber) + "/page/" + _listView.currentPage;
            //                source: "image://poppler" + (index % poppler.providersNumber) + "/page/" + index;
            source: model.url
            //                                source: "image://poppler/page/" + _listView.currentPage;
            // sourceSize.width: model.width * (contentWidth/model.width)
            // sourceSize.height: model.height * (contentHeight/model.height) 
            sourceSize.width: model.width * (1000/model.width)
            sourceSize.height: model.height * (1000/model.height)
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

            Item
            {
                id: _selectionLayer
                
                parent: pageImg.image
                height: pageImg.image.paintedHeight
                width: pageImg.image.paintedWidth
                anchors.centerIn: parent
                
                
                Rectangle
                {
                    visible: __currentSearchResult.page === index
                    color: control.searchHighlightColor
                    x: Math.round(__currentSearchResult.rect.x * pageImg.image.paintedWidth) 
                    y: Math.round(__currentSearchResult.rect.y * pageImg.image.paintedHeight) 
                    width: Math.round(__currentSearchResult.rect.width *  pageImg.image.paintedWidth)
                    height: Math.round(__currentSearchResult.rect.height *  pageImg.image.paintedHeight)
                }
                
               
               Maui.ContextualMenu
               {
                   id: _menu
                   property string selectedText
                   MenuItem
                   {
                           icon.name: "edit-copy-symbolic"
                           text: i18n("Copy")
                                              
                       onTriggered:
                       {
                           Maui.Handy.copyTextToClipboard(_menu.selectedText)
                       }
                       
                       enabled: _menu.selectedText.length
                   } 
                   
                   MenuItem
                   {
                       text: i18n("Search")
                }
                
                MenuItem
                {
                    text: i18nd("mauikitdocuments","Search Selected Text on Google...")
                    onTriggered: Qt.openUrlExternally("https://www.google.com/search?q="+_menu.selectedText)
                    enabled: _menu.selectedText.length
                }
                
                onClosed: selectLayer.reset()
            }
                
                Loader
                {
                    asynchronous: true
                    active: control.enableLassoSelection && pageImg.ListView.isCurrentItem && !pageImg.zooming
                    //                active: !Maui.Handy.hasTransientTouchInput && !Maui.Handy.isMobile
                    anchors.fill: parent
                    // z: parent.z-1
                    clip: false
                    
                    sourceComponent: MouseArea
                    {
                        id: _mouseArea
                        
                        propagateComposedEvents: true
                        preventStealing: true
                        acceptedButtons: Qt.RightButton | Qt.LeftButton
                        scrollGestureEnabled: false
                        
                        onClicked: (mouse) =>
                        {
                            console.log("Area clicked")
                            control.areaClicked(mouse)
                            control.forceActiveFocus()
                            
                            if(mouse.button === Qt.RightButton)
                            {
                                control.areaRightClicked()
                                return
                            }
                        }
                        
                        onPositionChanged: (mouse) =>
                        {                           
                            if(_mouseArea.pressed && control.enableLassoSelection && selectLayer.visible)
                            {
                                if(mouseX >= selectLayer.newX)
                                {
                                    selectLayer.width = (mouseX + 10) < (control.x + control.width) ? (mouseX - selectLayer.x) : selectLayer.width;
                                } else {
                                    selectLayer.x = mouseX < control.x ? control.x : mouseX;
                                    selectLayer.width = selectLayer.newX - selectLayer.x;
                                }
                                
                                if(mouseY >= selectLayer.newY) {
                                    selectLayer.height = (mouseY + 10) < (control.y + control.height) ? (mouseY - selectLayer.y) : selectLayer.height;
                                    // if(!controlView.atYEnd &&  mouseY > (control.y + control.height))
                                    // controlView.contentY += 10
                                } else {
                                    selectLayer.y = mouseY < control.y ? control.y : mouseY;
                                    selectLayer.height = selectLayer.newY - selectLayer.y;
                                    
                                    // if(!controlView.atYBeginning && selectLayer.y === 0)
                                    // controlView.contentY -= 10
                                }
                            }
                        }
                        
                        onPressed: (mouse) =>
                        {
                            if (mouse.source === Qt.MouseEventNotSynthesized)
                            {
                                if(control.enableLassoSelection && mouse.button === Qt.LeftButton)
                                {
                                    selectLayer.visible = true;
                                    selectLayer.x = mouseX;
                                    selectLayer.y = mouseY;
                                    selectLayer.newX = mouseX;
                                    selectLayer.newY = mouseY;
                                    selectLayer.width = 0
                                    selectLayer.height = 0;
                                }
                            }
                        }
                        
                        onPressAndHold: (mouse) =>
                        {
                            if ( mouse.source !== Qt.MouseEventNotSynthesized && control.enableLassoSelection && !selectLayer.visible )
                            {
                                selectLayer.visible = true;
                                selectLayer.x = mouseX;
                                selectLayer.y = mouseY;
                                selectLayer.newX = mouseX;
                                selectLayer.newY = mouseY;
                                selectLayer.width = 0
                                selectLayer.height = 0;
                                
                                mouse.accepted = true
                            }else
                            {
                                mouse.accepted = false
                            }
                        }
                        
                        onReleased: (mouse) =>
                        {
                            if(mouse.button !== Qt.LeftButton || !control.enableLassoSelection || !selectLayer.visible)
                            {
                                mouse.accepted = false
                                return;
                            }
                            
                            // if(selectLayer.y > controlView.contentHeight)
                            // {
                            //     return selectLayer.reset();
                            // }
                            
                            // var lassoIndexes = []
                            // const limitX = mouse.x === lassoRec.x ? lassoRec.x+lassoRec.width : mouse.x
                            // const limitY =  mouse.y === lassoRec.y ?  lassoRec.y+lassoRec.height : mouse.y
                            
                            control.areaSelected(Qt.rect(selectLayer.x, selectLayer.y, selectLayer.width, selectLayer.height))
                            
                            console.log("Area selected", Qt.rect(selectLayer.x, selectLayer.y, selectLayer.width, selectLayer.height))
                            
                            console.log(_listView.currentItem.image.paintedHeight, _listView.currentItem.image.paintedWidth,_listView.currentItem.width, (_listView.currentItem.width/2)-(_listView.currentItem.image.paintedWidth/2))
                            
                            if(selectLayer.width > 0 && selectLayer.height > 0)
                            {
                            _menu.selectedText = poppler.getText(Qt.rect(selectLayer.x, selectLayer.y, selectLayer.width, selectLayer.height), Qt.size(pageImg.image.paintedWidth, pageImg.image.paintedHeight), pageImg.page)
                            _menu.show()
                                                            }
                            //                 for(var i =lassoRec.x; i < limitX; i+=(lassoRec.width/(controlView.cellWidth* 0.5)))
                            //                 {
                            //                     for(var y = lassoRec.y; y < limitY; y+=(lassoRec.height/(controlView.cellHeight * 0.5)))
                            //                     {
                            //                         const index = controlView.indexAt(i,y+controlView.contentY)
                            //                         if(!lassoIndexes.includes(index) && index>-1 && index< controlView.count)
                            //                             lassoIndexes.push(index)
                            //                     }
                            //                 }
                            //                 
                            //                 if(lassoIndexes.length > 0)
                            //                 {
                            //                     control.itemsSelected(lassoIndexes)
                            //                 }
                            
                            
                        }
                    }
                }
                
                Label
                {
                    visible: selectLayer.width > 0 && selectLayer.height > 0 && selectLayer.visible && !_menu.visible
                    Maui.Theme.colorSet: Maui.Theme.Complementary
                    Maui.Theme.inherit: false
                    padding: Maui.Style.defaultPadding
                    anchors.bottom: selectLayer.top
                    anchors.margins: Maui.Style.space.big
                    anchors.left: selectLayer.left
                    
                                        text: poppler.getText(Qt.rect(selectLayer.x, selectLayer.y, selectLayer.width, selectLayer.height), Qt.size(pageImg.image.paintedWidth, pageImg.image.paintedHeight), pageImg.page)
                    
                    background: Rectangle
                    {
                        color: Maui.Theme.backgroundColor
                        radius: Maui.Style.radiusV
                    }
                }
                
                Maui.Rectangle
                {
                    id: selectLayer
                    property int newX: 0
                    property int newY: 0
                    height: 0
                    width: 0
                    x: 0
                    y: 0
                    visible: false
                    color: Qt.rgba(control.Maui.Theme.highlightColor.r,control.Maui.Theme.highlightColor.g, control.Maui.Theme.highlightColor.b, 0.2)
                    opacity: 0.7
                    
                    borderColor: control.Maui.Theme.highlightColor
                    borderWidth: 2
                    solidBorder: false
                    /*
                    Text
                    {
                        color: "yellow"
                        text: selectLayer.x + " / " + selectLayer.y + " // " + selectLayer.width  + " // " + selectLayer.height 
                    }*/
                    
                    function reset()
                    {
                        selectLayer.x = 0;
                        selectLayer.y = 0;
                        selectLayer.newX = 0;
                        selectLayer.newY = 0;
                        selectLayer.visible = false;
                        selectLayer.width = 0;
                        selectLayer.height = 0;
                    }
                    
                    
                }
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
