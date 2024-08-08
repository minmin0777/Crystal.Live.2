
/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-07-16 14:08:11
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-17 13:27:23
 * @FilePath: \Crystal.Live.2\src\Simulator\src\MyCppClass.cpp
 * @Description:
 * @
 * @#|----------------------------------------------------------------------------|
 * @#|  Remark         : Description                                              |
 * @#|----------------------------------------------------------------------------|
 * @#|  Change History :                                                          |
 * @#|  <Date>     | <Version> | <Author>       | <Description>                   |
 * @#|----------------------------------------------------------------------------|
 * @#|  2024/05/02 | 0.0.0.1   | jason.chen     | Create file                     |
 * @#|----------------------------------------------------------------------------|
 * @#|                                                                            |
 * @#|----------------------------------------------------------------------------|
 * @Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 ————————————————————————————————————————————————————————————————————————————————————————*/
 // MyCppClass.cpp
#include "MyCppClass.h"
#include <QDebug>




MyCppClass::MyCppClass(QObject* parent) : QObject(parent)
{
    m_pSipServiceThreadWrapper = nullptr;
    //设置回调函数
    m_SipUdpClientThreadWrapper.setOnMessageSentCallback(std::bind(&MyCppClass::onMessageSent, this, std::placeholders::_1));

}


void MyCppClass::myFunction()
{
    qDebug() << "Called from QML";
}

std::string replaceNewlineWithCRLF(const std::string& input) {
    std::string result;
    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] == '\n' && (i == 0 || input[i - 1] != '\r')) {
            result += "\r\n";
        }
        else {
            result += input[i];
        }
    }
    return result;
}

Q_INVOKABLE bool MyCppClass::SendSipMessage(QString strSipMessage, QString strRemoteAddress)
{
    //清空输出框的内容
    m_OutputText.clear();
    setOutputText("");
    std::string strIP = strRemoteAddress.indexOf(":") > 0 ? strRemoteAddress.left(strRemoteAddress.indexOf(":")).toLocal8Bit().toStdString() : strRemoteAddress.toLocal8Bit().toStdString();
    uint32_t port = strRemoteAddress.indexOf(":") > 0 ? strRemoteAddress.right(strRemoteAddress.length() - strRemoteAddress.indexOf(":") - 1).toUInt() : 5060;

    std::string strMessage = strSipMessage.toStdString().c_str();

    std::string modified = replaceNewlineWithCRLF(strMessage);
    m_SipUdpClientThreadWrapper.sendSipMessagebyUdp2(strIP, port, modified.c_str(), modified.length());
    //m_SipUdpClientThreadWrapper.sendSipMessagebyUdp(modified.c_str(), modified.length());
    // setOutputText(stroutput);
    //m_SipUdpClientThreadWrapper.Start()
        //TODO: Send message to SIP server
        // auto* crash = reinterpret_cast<volatile int*>(0);
        // *crash = 0;
    return  true;

}

Q_INVOKABLE int MyCppClass::StartSipService(int port)
{



    //创建线程并记录起来
    m_pSipServiceThreadWrapper =
        std::make_shared<CSipServiceThreadWrapper>();


    return true;
}

QString MyCppClass::getOutputText() const
{
    return m_OutputText;
}

void MyCppClass::setOutputText(const QString& text)
{
    if (m_OutputText.isEmpty())
    {
        m_OutputText = text;
    }
    else
    {
        m_OutputText += "-----------------------------------------------------\n";
        m_OutputText += text;
    }
    emit OutputTextChanged(m_OutputText);
}

size_t MyCppClass::onMessageSent(const QString& strMessage)
{
    setOutputText(strMessage);
    return 0;
}

// void MyCppClass::updateText(const QString& text)
// {

// }