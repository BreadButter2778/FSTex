#include "RefereeSystem.h"

#include <iostream>   // std::cout
#include <stdexcept>  // std::invalid_argument, std::logic_error
#include <utility>    // std::move

namespace rm {

// registerRobot()：登记机器人（合法性检查 + 所有权移交）
void RefereeSystem::registerRobot(std::unique_ptr<Robot> robot) {
    if (!robot) {                              // 1. 空指针检查
        throw std::invalid_argument("机器人不能为空");
    }
    if (robot->info().id <= 0) {               // 2. 编号非法检查
        throw std::invalid_argument("编号非法");
    }
    if (robot->info().name.empty()) {          // 3. 名称非空检查
        throw std::invalid_argument("名称不能为空");
    }
    for (const auto& existing : robots_) {     // 4. 编号重复检查
        if (existing->info().id == robot->info().id) {
            throw std::invalid_argument("机器人编号重复");
        }
    }
    robots_.push_back(std::move(robot));       // 5. 全部通过，所有权移交给裁判
}

// findRobot()：按编号查找机器人，找不到返回 nullptr
Robot* RefereeSystem::findRobot(int id) {
    for (const auto& r : robots_) {            // 遍历名单
        if (r->info().id == id) {              // 编号匹配
            return r.get();                    // 返回裸指针（非拥有，只借用）
        }
    }
    return nullptr;                            // 没找到
}

// processEvent()：处理一条事件，返回实际扣除/恢复量，失败抛异常
int RefereeSystem::processEvent(const MatchEvent& event) {
    Robot* robot = findRobot(event.robotId);   // 先按编号找机器人
    if (!robot) {                              // 找不到
        throw std::invalid_argument("机器人不存在");
    }

    int result = 0;
    switch (event.type) {
    case EventType::Damage:                    // 伤害：调 takeDamage（内部校验 value>0）
        result = robot->takeDamage(event.value);
        break;
    case EventType::Supply:                    // 补给：多态调用派生类的 supply
        result = robot->supply();
        break;
    default:                                   // 未知事件类型
        throw std::invalid_argument("未知的事件类型");
    }

    ++successfulEvents_;                       // 走到这里才算成功
    return result;
}

// successfulEvents()：返回成功事件数（只读）
std::size_t RefereeSystem::successfulEvents() const {
    return successfulEvents_;
}

// printStatus()：按登记顺序输出所有机器人状态
void RefereeSystem::printStatus() const {
    for (const auto& r : robots_) {            // vector 顺序即登记顺序
        std::cout << r->info().id << ' '
                  << (r->info().team == Team::Red ? "红方" : "蓝方") << ' '
                  << r->typeName() << ' '
                  << r->info().name << ' '
                  << r->hp() << '/' << r->maxHp() << ' '
                  << (r->isAlive() ? "正常" : "已退场") << '\n';
    }
}

} // namespace rm
