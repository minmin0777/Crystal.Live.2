/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-07-16 14:07:41
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-23 14:56:46
 * @FilePath: \Crystal.Live.2\src\Simulator\src\MyCppClass.h
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

 // MyCppClass.h
#pragma once
#ifndef MYCPPCLASS_H
#define MYCPPCLASS_H

#include <QObject>
#ifndef _WINSOCK2API_
#include <winsock2.h>
#endif
#include "hv/hloop.h"
#include "hv/hsocket.h"
#include <memory>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "CSipServiceThreadWrapper.h"
#include "CSipUdpClientThreadWrapper.h"
class MyCppClass : public QObject
{
    Q_OBJECT
public:
    explicit MyCppClass(QObject* parent = nullptr);
    /**
     * @brief :Get the Output Text object
     * @remark:设置QML输出对话框(OutputTextBox)的交互属性OutputText
     * @remark:以及对应的读写函数(set/getOutputText)与信号函数(OutputTextChanged)
     */
    Q_PROPERTY(QString OutputText READ getOutputText WRITE setOutputText NOTIFY OutputTextChanged)

public:

public:
    Q_INVOKABLE void myFunction();
    Q_INVOKABLE bool SendSipMessage(QString strSipMessage, QString strRemoteAddress);
    Q_INVOKABLE int StartSipService(int port);

    QString getOutputText() const;
    void setOutputText(const QString& text);
signals:
    void OutputTextChanged(const QString& text);
private:
    std::shared_ptr<CSipServiceThreadWrapper> m_pSipServiceThreadWrapper;
    CSipUdpClientThreadWrapper m_SipUdpClientThreadWrapper;
private:
    QString m_OutputText;
public:
    size_t onMessageSent(const QString& strMessage);
};

#endif // MYCPPCLASS_H