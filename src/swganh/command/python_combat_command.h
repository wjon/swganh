// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMBAT_PYTHON_COMBAT_COMMAND_H_
#define SWGANH_COMBAT_PYTHON_COMBAT_COMMAND_H_

#ifndef WIN32
#include <Python.h>
#endif

#include <cstdint>
#include <memory>
#include <string>

#include "swganh/scripting/python_script.h"

#include "command_properties.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"

namespace swganh {
namespace app {
    class SwganhKernel;
}}

namespace swganh {
namespace object {
	class Object;
namespace tangible {
    class Tangible;
}
namespace creature {
	class Creature;
}}

namespace command {

class PythonCombatCommand
{
public:
    PythonCombatCommand(const CommandProperties& command_properties);

    boost::python::object operator()(swganh::app::SwganhKernel* kernel, std::shared_ptr<swganh::object::creature::Creature> actor, std::shared_ptr<swganh::object::tangible::Tangible> target, const swganh::messages::controllers::CommandQueueEnqueue& command_queue_message);
private:
    swganh::scripting::PythonScript script_;
};


}}  // namespace swganh::command

#endif  // SWGANH_COMBAT_PYTHON_COMBAT_COMMAND_H_
