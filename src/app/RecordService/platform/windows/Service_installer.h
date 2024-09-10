/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-27 14:44:17
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-27 16:00:18
 * @FilePath: \Crystal.Live.2\src\app\RecordService\platform\windows\Service_installer.h
 * @Description:
 * @
 * @#|----------------------------------------------------------------------------|
 * @#|  ClassName         : Description                                           |
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
#pragma once
#ifndef SERVICE_INSTALLER_H_
#define SERVICE_INSTALLER_H_

#include "service_base.h"

#define SVC_ERROR ((DWORD)0xC0020001L)
#define SVCNAME TEXT("CrystalLive.RecordService")

class ServiceInstaller {
public:
	static bool Install(const ServiceBase& service);
	static bool Uninstall(const ServiceBase& service);
private:
	ServiceInstaller() {}
};

#endif // SERVICE_INSTALLER_H_