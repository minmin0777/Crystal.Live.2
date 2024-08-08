import QtQuick 2.15
import QtQuick.Window 2.15
import FluentUI 1.0
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import com.mycompany.MyCppClass 1.0

FluWindow {
    id:testSipWindow
    objectName: "root"
    width: 1400
    height: 1000
    minimumWidth: 1080
    minimumHeight: 680
    title: qsTr("Simulator")
    Component.onCompleted: {
        console.log("窗口加载完成")
        FluTheme.darkMode = FluThemeType.Dark
        FluTheme.animationEnabled = true
    }
    MyCppClass {
        id: myCppClassInstance
        // 设置属性或调用方法
    }
    FluText
    {
        id:inputText
        width: (parent.width -60)/2
        text: qsTr("Input content:")
        anchors{
            topMargin: 10
            leftMargin: 10
            top: parent.top
            left: parent.left
        }
    }


    Item {

        id:inputItem
        objectName: "inputItem"
        width: (parent.width -60)/2
        height:(parent.height -150)
        anchors{
            topMargin: 10
            leftMargin: 0
            top: inputText.bottom
            left: inputText.left
        }

        ScrollView {
            id: inputScrollView
            objectName: "inputScrollView"
            anchors.fill: parent
            FluMultilineTextBox{
                id:inputTextBox
                objectName: "inputTextBox"
                anchors.fill: inputItem
                placeholderText:qsTr("input content Edit")
               
            }  
           
            // 自定义滚动条样式
            FluScrollBar.vertical: FluScrollBar 
            {
                //policy: FluScrollBar.AlwaysOn
                height:parent.height
                anchors.right: parent.right

            }
        }
    }
    
    
    FluText
    {
        id:outputText
        width: (parent.width -60)/2
        text: qsTr("output content:")
        anchors{
            topMargin: 10
            rightMargin: 0
            top: parent.top
            right: parent.right
        }
    }

    Item {

        id:outputItem
        width: (parent.width -60)/2
        height:(parent.height -150)
        anchors{
            topMargin: 10
            rightMargin: 10
            top: outputText.bottom
            right: outputText.right
        }

        ScrollView {
            id: outputScrollView
            anchors.fill: parent
            FluMultilineTextBox{
                id:outputTextBox
                anchors.fill: outputItem
                placeholderText:qsTr("output content Edit(RedOnly)")
                readOnly: true  //设置文本框为只读
                Component.onCompleted: {
                    myCppClassInstance.OutputTextChanged.connect(function(text) {

                        outputTextBox.text = text;
                        console.log("文本已更改: " + text); // 添加这行代码来打印日志
                    });
    }
            }  
           
            // 自定义滚动条样式
            FluScrollBar.vertical: FluScrollBar 
            {
                //policy: FluScrollBar.AlwaysOn
                height:parent.height
                anchors.right: parent.right

            }
        }
    }

    Rectangle {
        width: 1 // 分割线的宽度为1像素
        height: (outputTextBox.height-20 )// 让分割线的高度与父元素的高度相同
        color: "#b7b7b7" // 分割线的颜色，这里使用浅灰色
        anchors{
            top: outputItem.top
            topMargin: 10
            rightMargin: 0
            horizontalCenter: parent.horizontalCenter
        
        }

    }
  
   
    
    

        FluFrame{
            id:frameBottom
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            padding: 10
            width: (parent.width-80)
            height: 80
            anchors
            {
                bottom: parent.bottom
                bottomMargin: 14
                horizontalCenter: parent.horizontalCenter
                left: inputItem.left
                right: outputItem.right
            }
            Row{
                spacing: 30
                anchors.verticalCenter: parent.verticalCenter
                FluText{
                  id:label_RemoteAddress
                   anchors
                   {
                       verticalCenter: parent.verticalCenter
                       left: parent.left
                   }
                   text: qsTr("Remote Address:")
                }
                FluTextBox{
                    
                    id:input_RemoteAddress
                    anchors
                    {
                       verticalCenter: parent.verticalCenter
                       left:label_RemoteAddress.right
                       leftMargin:20 
                    }
                    width: frameBottom.width * 1/3
                    objectName: "input_RemoteAddress"
                    placeholderText:"Please input remote Address(Example:127.0.0.1:5060)"
                    text:"127.0.0.1:5060"
               
                } 

                FluToggleSwitch
                {
                    id:switch_UseReturn
                    text: qsTr("Enable")
                    checked: true
                    anchors
                    {
                        verticalCenter: parent.verticalCenter
                        left: input_RemoteAddress.right
                        leftMargin: 50
                        
                    }
                    onCheckedChanged:
                    {
                        if (checked)
                        {
                            text: qsTr("Enable")
                            console.log("checked")
                        }
                        else
                        {
                            text: qsTr("disable")
                            console.log("unchecked")
                        }
                    }
                
                }


            }

            Row{
                spacing: 20 // 设置组件之间的间距
                anchors
                {
                    //horizontalCenter: parent.horizontalCenter
                    right: parent.right
                    left: parent.width * 3/5
                    rightMargin:20
                    verticalCenter: parent.verticalCenter
                }

                FluFilledButton
                {
                    id:btnTestSipMessage
                    text: qsTr("Test Sip Message")

                    onClicked:
                    {
                        console.log("inputTextBox text:", top)
                        var strMessage = inputTextBox.text
                        var strAddress = input_RemoteAddress.text
                        if (myCppClassInstance.SendSipMessage(strMessage,strAddress) == true)
                        {
                            showInfo("Sip Message sent", 3)
                          
                        }
                        myCppClassInstance.myFunction()
                    }
                }

                FluFilledButton
                {
                    id:btnStartSipServer
                    text: qsTr("Start Sip Server ")

                    onClicked:
                    {
                        
                        if (myCppClassInstance.StartSipService(5060) == true)
                        {
                            var info1 = showInfo(qsTr("Start Sip Server success(5060)'"), 0)
                        }

                    }
                }

                FluFilledButton{
                    text: qsTr("Clear")
                    onClicked: {
                        inputTextBox.text = ""
                        outputTextBox.text = ""
                    }
                }
                FluFilledButton{
                    text: qsTr("Exit")
                    onClicked: {
                        Qt.quit()
                    }

                }
            
            }
        
        }


}
