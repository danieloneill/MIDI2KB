import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import Qt.labs.qmlmodels 1.0
import Qt.labs.settings 1.0

Rectangle {
    id: top
    property variant notes: ({})

    ListModel {
        id: keys

        ListElement { display:'A';    value:0x41 }
        ListElement { display:'B';    value:0x42 }
        ListElement { display:'C';    value:0x43 }
        ListElement { display:'D';    value:0x44 }
        ListElement { display:'E';    value:0x45 }
        ListElement { display:'F';    value:0x46 }
        ListElement { display:'G';    value:0x47 }
        ListElement { display:'H';    value:0x48 }
        ListElement { display:'I';    value:0x49 }
        ListElement { display:'J';    value:0x4A }
        ListElement { display:'K';    value:0x4B }
        ListElement { display:'L';    value:0x4C }
        ListElement { display:'M';    value:0x4D }
        ListElement { display:'N';    value:0x4E }
        ListElement { display:'O';    value:0x4F }
        ListElement { display:'P';    value:0x50 }
        ListElement { display:'Q';    value:0x51 }
        ListElement { display:'R';    value:0x52 }
        ListElement { display:'S';    value:0x53 }
        ListElement { display:'T';    value:0x54 }
        ListElement { display:'U';    value:0x55 }
        ListElement { display:'V';    value:0x56 }
        ListElement { display:'W';    value:0x57 }
        ListElement { display:'X';    value:0x58 }
        ListElement { display:'Y';    value:0x59 }
        ListElement { display:'Z';    value:0x5A }
        ListElement { display:'LShift'; value:0xA0 }
        ListElement { display:'Space'; value:0x20 }
    }

    Settings {
        id: settings
        property variant mappings

        function commit()
        {
/*
            TableModelColumn { display: 'note' }
            TableModelColumn { display: 'velocity' }
            TableModelColumn { display: 'count' }
            TableModelColumn { display: 'enabled' }
            TableModelColumn { display: 'key' }
            TableModelColumn { display: 'threshold' }
            TableModelColumn { display: 'tag' }
*/
            var tosave = [];
            for( var x=0; x < eventsModel.rowCount; x++ )
            {
                var idx = eventsModel.index( x, 0 );
                var note = eventsModel.data( idx, 'display' );
                idx = eventsModel.index( x, 3 );
                var enabled = eventsModel.data( idx, 'display' );
                idx = eventsModel.index( x, 4 );
                var key = eventsModel.data( idx, 'display' );
                idx = eventsModel.index( x, 5 );
                var threshold = eventsModel.data( idx, 'display' );
                idx = eventsModel.index( x, 6 );
                var tag = eventsModel.data( idx, 'display' );

                //console.log("Row: "+x+": "+note+" ("+enabled+") Key:"+key+"/Threshold:"+threshold+" -> "+tag);
                tosave.push({'note':note, 'enabled':enabled, 'key':key, 'threshold':threshold, 'tag':tag});
            }
            settings.mappings = tosave;
        }

        function restore()
        {
            eventsModel.clear();
            top.notes = {};
            try {
                for( var x=0; x < mappings.length; x++ )
                {
                    var m = mappings[x];
                    notes[m.note] = { 'position':x, 'count':1 };
                    eventsModel.appendRow({'note':''+m.note, 'velocity':''+0, 'count':'0', 'enabled':m.enabled, 'key':m.key, 'threshold':m.threshold, 'tag':m.tag});
                }
            } catch(e) {
                console.log("Failed to restore settings: "+e);
            }
        }
    }

    Component.onCompleted: settings.restore();

    Connections {
        target: MainWindow

        function onMidiEvent(note, velocity)
        {
            if( notes[note] )
            {
                // Adjust
                var nent = notes[note];
                nent['count']++;
                nent['velocity'] = velocity;
                notes[note] = nent;
                var idx = eventsModel.index( nent['position'], 2 );
                eventsModel.setData( idx, 'display', ''+nent['count'] );
                idx = eventsModel.index( nent['position'], 1 );
                eventsModel.setData( idx, 'display', ''+nent['velocity'] );

                // Enabled? If so, trigger the keypress:
                if( MainWindow.active )
                {
                    idx = eventsModel.index( nent['position'], 3 );
                    var enabled = eventsModel.data( idx, 'display' );
                    if( !enabled )
                        return;

                    idx = eventsModel.index( nent['position'], 5 );
                    var threshold = eventsModel.data( idx, 'display' );
                    if( velocity < threshold )
                        return;

                    idx = eventsModel.index( nent['position'], 4 );
                    var key = eventsModel.data( idx, 'display' );
                    MainWindow.queueKey(key);
                }
            }
            else
            {
                notes[note] = { 'position':eventsModel.rowCount, 'count':1 };
                eventsModel.appendRow({'note':''+note, 'velocity':''+velocity, 'count':'1', 'enabled':false, 'key':0x20, 'threshold':0, 'tag':''});
            }
        }

        function onResetClicked()
        {
            eventsModel.clear();
            top.notes = {};
        }
/*
        function onActiveChanged()
        {
            console.log("Active: "+MainWindow.active);
        }
*/
    }

    TableView {
        id: eventsListView
        anchors.fill: parent

        columnSpacing: 10
        rowSpacing: 5
        boundsBehavior: Flickable.StopAtBounds

        topMargin: 30
        model: TableModel {
            id: eventsModel
            TableModelColumn { display: 'note' }
            TableModelColumn { display: 'velocity' }
            TableModelColumn { display: 'count' }
            TableModelColumn { display: 'enabled' }
            TableModelColumn { display: 'key' }
            TableModelColumn { display: 'threshold' }
            TableModelColumn { display: 'tag' }
        }
        delegate: DelegateChooser {
            DelegateChoice {
                column: 3
                delegate: Item {
                    implicitWidth: 64
                    implicitHeight: 30
                    CheckBox {
                        anchors.centerIn: parent
                        checked: model.display
                        onToggled: {
                            model.display = checked
                            settings.commit();
                        }
                    }
                }
            }
            DelegateChoice {
                column: 4
                delegate: ComboBox {
                    //currentIndex: indexOfValue(display)
                    property bool starting: true
                    implicitWidth: 96
                    onCurrentIndexChanged: {
                        if( starting )
                            return;

                        display = keys.get( currentIndex ).value;
                        settings.commit();
                    }

                    Component.onCompleted: {
                        // A hack to set the currentIndex:
                        for( var p=0; p < keys.count;p++ )
                        {
                            var k = keys.get(p);
                            if( k.value == display )
                            {
                                starting = false;
                                return currentIndex = p;
                            }
                        }
                        starting = false;
                    }

                    textRole: 'display'
                    valueRole: 'value'
                    model: keys
                }
            }
            DelegateChoice {
                column: 5
                delegate: SpinBox {
                    implicitWidth: 128
                    value: model.display
                    editable: true
                    from: 0
                    to: 127
                    onValueModified: {
                        model.display = value;
                        settings.commit();
                    }
                }
            }
            DelegateChoice {
                column: 6
                delegate: TextField {
                    text: display
                    selectByMouse: true
                    implicitWidth: 96
                    onTextEdited: {
                        display = text;
                        settings.commit();
                    }
                }
            }
            DelegateChoice {
                delegate: Item {
                    implicitWidth: 64
                    implicitHeight: 30
                    Text {
                        anchors.centerIn: parent
                        text: model.display
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }

    Rectangle {
        anchors {
            top: header.top
            bottom: header.bottom
            left: header.left
            right: header.right
            bottomMargin: -1
            rightMargin: -1
            topMargin: -1
            leftMargin: -1
        }
        color: 'lightgray'
        border.color: 'black'
        border.width: 1
    }
    Row {
        id: header
        anchors {
            top: eventsListView.top
            left: eventsListView.left
            right: eventsListView.right
        }

        //spacing: 10
        height: 30
        width: eventsListView.width

        HeaderButton { width: 69; text: qsTr('Note') }
        HeaderButton { width: 74; text: qsTr('Velocity') }
        HeaderButton { width: 74; text: qsTr('Count') }
        HeaderButton { width: 74; text: qsTr('Enabled') }
        HeaderButton { width: 106; text: qsTr('Key') }
        HeaderButton { width: 138; text: qsTr('Threshold') }
        HeaderButton { width: 106; text: qsTr('Tag') }
    }
}
