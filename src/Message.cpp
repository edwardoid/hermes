/**
 * Hermes - A RPC for IOT
 * Copyright (C) 2023  Eduard Sargsyan and Andrey Ovodov
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <hermes/Message.h>

const char* hermes::mt2str(const MessageType& type)
{
    #define MT2_STR_HELPER(Type) case MessageType:: Type: { return #Type; }
    switch (type)
    {
        MT2_STR_HELPER(Error)
        MT2_STR_HELPER(Handshake)
        MT2_STR_HELPER(Command)
        MT2_STR_HELPER(InternalError)
    default:
        break;
    }
    #undef MT2_STR_HELPER
    return "UnknownType";
}

const char* hermes::vd2str(const hermes::ValueData& vd, char* str)
{
    switch (vd.type)
    {
    case hermes::ValueType::Boolean:
    {
        strncpy(str, vd.value.B ? "true\0" : "false", 6);
        break;
    }
    case hermes::ValueType::Float:
    {
        snprintf(str, HERMES_STRING_LENGTH, "%f", float(vd.value.F.V) / float(vd.value.F.Precision));
        break;
    }
    case hermes::ValueType::Integer:
    {
        snprintf(str, HERMES_STRING_LENGTH, "%d", vd.value.I);
        break;
    }
    case hermes::ValueType::UnsignedInteger:
    {
        snprintf(str, HERMES_STRING_LENGTH, "%u", vd.value.U);
        break;
    }
    case hermes::ValueType::String:
    {
        strcpy(str, vd.value.S);
        break;
    }
    default: {
        return nullptr;
    }
    }
    
    return str;
}

const char* hermes::vd2str(const hermes::ValueData& vd)
{
    static char val[HERMES_STRING_LENGTH];
    memset(val, '\0', HERMES_SERIAL_LENGTH);
    if(vd2str(vd, val) == nullptr)
    {
        return "";
    }
    return val;
}

const char* hermes::cmd2str(const Command& cmd)
{
    #define CMD2_STR_HELPER(Cmd) case Command:: Cmd: { return #Cmd; }
    switch (cmd)
    {
        CMD2_STR_HELPER(Disconnect)
        CMD2_STR_HELPER(Set)
        CMD2_STR_HELPER(Get)
        CMD2_STR_HELPER(GetPropertiesCount)
        CMD2_STR_HELPER(GetPropertyName)
        CMD2_STR_HELPER(PollEvents)
    default:
        break;
    }
    #undef CMD2_STR_HELPER
    return "UnknownCommand";
}