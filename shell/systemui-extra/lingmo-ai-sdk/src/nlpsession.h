/*
 * Copyright 2024 KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef NLPSESSION_H
#define NLPSESSION_H

#include <jsoncpp/json/json.h>
#include <any>
#include <jsoncpp/json/value.h>
#include "nlp.h"
#include "services/textprocessorproxy.h"

namespace nlp {

class NlpSession
{
public:
    NlpResult init(const std::string &engineName, const Json::Value &config);

    void setResultCallback(NlpResultCallback callback, std::any userData);
    void setActor(int actorId);
    NlpResult chat(const std::string &message);
    void stopChat();

    void setContext(int size);
    void clearContext();

private:
    TextProcessorProxy textProcessor_;
};

}

#endif
