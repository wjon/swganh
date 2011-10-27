
#include "swganh/object/creature/creature_factory.h"

#include <sstream>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include <glog/logging.h>

#include "anh/database/database_manager.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/exception.h"

using namespace std;
using namespace anh::database;
using namespace swganh::object;
using namespace swganh::object::creature;


CreatureFactory::CreatureFactory(const shared_ptr<DatabaseManagerInterface>& db_manager)
    : TangibleFactory(db_manager)
{
}

void CreatureFactory::LoadTemplates()
{}

bool CreatureFactory::HasTemplate(const string& template_name)
{
    return false;
}

void CreatureFactory::PersistObject(const shared_ptr<Object>& object)
{}

void CreatureFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> CreatureFactory::CreateObjectFromStorage(uint64_t object_id)
{
    auto creature = make_shared<Creature>();
    creature->SetObjectId(object_id);
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::Statement>(conn->createStatement());
        shared_ptr<sql::ResultSet> result;
        stringstream ss;
        ss << "CALL sp_GetCreature(" << object_id << ");" ;
        statement->execute(ss.str());
        CreateBaseTangible(creature, statement);
        
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
                creature->owner_id_ = result->getUInt64("owner_id");
                creature->bank_credits_ = result->getUInt("bank_credits");
                creature->cash_credits_ = result->getUInt("cash_credits");
                creature->posture_ = (Posture)result->getUInt("posture");
                creature->faction_rank_ = result->getUInt("faction_rank");
                creature->scale_ = result->getDouble("scale");
                creature->battle_fatigue_ = result->getUInt("battle_fatigue");
                creature->state_bitmask_ = result->getUInt("state");
                creature->listen_to_id_ = result->getUInt64("musician_id");

                creature->stat_current_list_.Set(creature::HEALTH, Stat(result->getUInt("current_health")));
                creature->stat_current_list_.Set(creature::STRENGTH, Stat(result->getUInt("current_strength")));
                creature->stat_current_list_.Set(creature::CONSTITUTION, Stat(result->getUInt("current_constitution")));
                creature->stat_current_list_.Set(creature::ACTION, Stat(result->getUInt("current_action")));
                creature->stat_current_list_.Set(creature::QUICKNESS, Stat(result->getUInt("current_quickness")));
                creature->stat_current_list_.Set(creature::STAMINA, Stat(result->getUInt("current_stamina")));
                creature->stat_current_list_.Set(creature::MIND, Stat(result->getUInt("current_mind")));
                creature->stat_current_list_.Set(creature::FOCUS, Stat(result->getUInt("current_focus")));
                creature->stat_current_list_.Set(creature::WILLPOWER, Stat(result->getUInt("current_willpower")));

                creature->stat_max_list_.Set(creature::HEALTH, Stat(result->getUInt("max_health")));
                creature->stat_max_list_.Set(creature::STRENGTH, Stat(result->getUInt("max_strength")));
                creature->stat_max_list_.Set(creature::CONSTITUTION, Stat(result->getUInt("max_constitution")));
                creature->stat_max_list_.Set(creature::ACTION, Stat(result->getUInt("max_action")));
                creature->stat_max_list_.Set(creature::QUICKNESS, Stat(result->getUInt("max_quickness")));
                creature->stat_max_list_.Set(creature::STAMINA, Stat(result->getUInt("max_stamina")));
                creature->stat_max_list_.Set(creature::MIND, Stat(result->getUInt("max_mind")));
                creature->stat_max_list_.Set(creature::FOCUS, Stat(result->getUInt("max_focus")));
                creature->stat_max_list_.Set(creature::WILLPOWER, Stat(result->getUInt("max_willpower")));
            }
        }
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return creature;
}

shared_ptr<Object> CreatureFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<Creature>();
}
