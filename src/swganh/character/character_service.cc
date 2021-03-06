// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/character/character_service.h"

#include <boost/lexical_cast.hpp>

#include "anh/logger.h"

#include <iomanip>

#include "anh/crc.h"

#include "anh/plugin/plugin_manager.h"

#include "anh/network/soe/session.h"
#include "anh/network/soe/server_interface.h"

#include "anh/service/service_manager.h"

#include "swganh/login/login_service.h"
#include "swganh/login/login_client.h"
#include "swganh/login/account.h"

#include "swganh/character/character_provider_interface.h"

#include "swganh/connection/connection_service.h"
#include "swganh/connection/connection_client.h"
#include "swganh/messages/heart_beat.h"

#include "swganh/messages/delete_character_reply_message.h"
#include "swganh/character/character_data.h"
#include "swganh/messages/client_create_character_success.h"
#include "swganh/messages/client_create_character_failed.h"
#include "swganh/messages/client_random_name_response.h"

#include "swganh/app/swganh_kernel.h"

using namespace anh;
using namespace anh::app;
using namespace anh::event_dispatcher;
using namespace std;
using namespace swganh::character;
using namespace swganh::connection;
using namespace swganh::login;
using namespace swganh::messages;

using swganh::app::SwganhKernel;

CharacterService::CharacterService(SwganhKernel* kernel)
    : kernel_(kernel)
{
    character_provider_ = kernel->GetPluginManager()->CreateObject<CharacterProviderInterface>("CharacterService::CharacterProvider");
}

CharacterService::~CharacterService() {}

service::ServiceDescription CharacterService::GetServiceDescription() {
    service::ServiceDescription service_description(
        "ANH Character Service",
        "character",
        "0.1",
        "127.0.0.1",
        0,
        0,
        0);

    return service_description;
}

void CharacterService::Start() {
    auto connection_service = kernel_->GetServiceManager()->GetService<ConnectionService>("ConnectionService");

    connection_service->RegisterMessageHandler(
        &CharacterService::HandleClientCreateCharacter_, this);

    connection_service->RegisterMessageHandler(
        &CharacterService::HandleClientRandomNameRequest_, this);

    auto login_service = kernel_->GetServiceManager()->GetService<LoginService>("LoginService");

    login_service->RegisterMessageHandler(
        &CharacterService::HandleDeleteCharacterMessage_, this);
}

void CharacterService::HandleClientCreateCharacter_(
    const shared_ptr<ConnectionClient>& client, 
    ClientCreateCharacter message) 
{    
    uint64_t character_id;
    string error_code;
	bool name_check;
	string name_check_error_code;
	string name = std::string(message.character_name.begin(), message.character_name.end());
	
	// Profanity/Reserve/Developer/ect... name check.
	tie(name_check, name_check_error_code) = character_provider_->IsNameAllowed(name);
	if(!name_check) // Failed Name Check
	{
		ClientCreateCharacterFailed failed;
		failed.stf_file = "ui";
		failed.error_string = name_check_error_code;
		client->SendTo(failed);
		return; // Bail out of character creation.
	}

    tie(character_id, error_code) = character_provider_->CreateCharacter(message, client->GetAccountId());

    // heartbeat to let the client know we're still here
    HeartBeat heartbeat;
    client->SendTo(heartbeat);

    if (error_code.length() > 0 && character_id == 0) {
        ClientCreateCharacterFailed failed;
        failed.stf_file = "ui";
        failed.error_string = error_code;
        client->SendTo(failed);
    } else {
        ClientCreateCharacterSuccess success;
        success.character_id = character_id;
        client->SendTo(success);
    }
}

void CharacterService::HandleClientRandomNameRequest_(
    const shared_ptr<ConnectionClient>& client, 
    ClientRandomNameRequest message)
{
    ClientRandomNameResponse response;
    response.player_race_iff = message.player_race_iff;

    response.random_name = character_provider_->GetRandomNameRequest(message.player_race_iff);
    if (response.random_name.length() > 0) {
        response.stf_file = "ui";
        response.approval_string = "name_approved";
    }

    client->SendTo(response);
}

void CharacterService::HandleDeleteCharacterMessage_(
    const shared_ptr<LoginClient>& login_client, 
    DeleteCharacterMessage message)
{
    DeleteCharacterReplyMessage reply_message;
    reply_message.failure_flag = 1;

    if (character_provider_->DeleteCharacter(message.character_id, login_client->GetAccount()->account_id())) {
        reply_message.failure_flag = 0;
    }

    login_client->SendTo(reply_message);
}
