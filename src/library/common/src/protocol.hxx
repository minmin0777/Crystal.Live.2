#pragma once


// /*
//  * @Author: jason minmin0777@126.com
//  * @Date: 2024-06-02 03:21:00
//  * @LastEditors: jason minmin0777@126.com
//  * @LastEditTime: 2024-06-02 03:21:13
//  * @FilePath: \Crystal.Live.2\src\common\protocol.hxx
//  * @Description:
//  *
//  * #|----------------------------------------------------------------------------|
//  * #|  Remark         : Description                                              |
//  * #|----------------------------------------------------------------------------|
//  * #|  Change History :                                                          |
//  * #|  <Date>     | <Version> | <Author>       | <Description>                   |
//  * #|----------------------------------------------------------------------------|
//  * #|  2024/05/02 | 0.0.0.1   | jason.chen     | Create file                     |
//  * #|----------------------------------------------------------------------------|
//  * #|                                                                            |
//  * #|----------------------------------------------------------------------------|
//  * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
//  */
// #include <osip2/osip.h>
// #include <osipparser2/sdp_message.h>
// #include <string>

// class SipParser {
// public:
//     SipParser() {
//         osip_init(&osip);
//     }

//     ~SipParser() {
//         osip_release(osip);
//     }

//     bool parse(const std::string& sipMessage) {
//         osip_message_t* sip;
//         if (osip_message_init(&sip) != 0) {
//             return false;
//         }

//         if (osip_message_parse(sip, sipMessage.c_str(), sipMessage.length()) != 0) {
//             osip_message_free(sip);
//             return false;
//         }

//         if (osip_message_is_invite(sip)) {
//             osip_body_t* body;
//             osip_message_get_body(sip, 0, &body);

//             char* body_str;
//             osip_body_to_str(body, &body_str);

//             sdp_message_t* sdp;
//             sdp_message_init(&sdp);
//             sdp_message_parse(sdp, body_str);

//             sdp_version = sdp->v_version;

//             sdp_message_free(sdp);
//             osip_free(body_str);
//         }

//         osip_message_free(sip);
//         return true;
//     }

//     std::string getSdpVersion() const {
//         return sdp_version;
//     }

// private:
//     osip_t* osip;
//     std::string sdp_version;
// };