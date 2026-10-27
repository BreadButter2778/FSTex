#include "RefereeSystem.h"

#include <iostream>  // std::cout
#include <memory>    // std::make_unique
#include <vector>    // std::vector

// 从 rm 命名空间引入要用的类型（main 函数本身留在全局命名空间）
using rm::AerialRobot;
using rm::EventType;
using rm::Hero;
using rm::Infantry;
using rm::MatchEvent;
using rm::RefereeSystem;
using rm::Robot;
using rm::RobotInfo;
using rm::Sentry;
using rm::Team;

int main() {
    RefereeSystem system;

    // === 登记机器人 ===
    std::cout << "=== 登记机器人 ===\n";
    system.registerRobot(std::make_unique<Infantry>(RobotInfo{1, "王八", Team::Red}));
    system.registerRobot(std::make_unique<Hero>(RobotInfo{2, "英雄", Team::Red}));
    system.registerRobot(std::make_unique<Sentry>(RobotInfo{3, "过洞", Team::Blue}));
    system.registerRobot(std::make_unique<AerialRobot>(RobotInfo{4, "无人机", Team::Blue}));
    system.printStatus();

    // 尝试登记重复编号，应失败
    try {
        system.registerRobot(std::make_unique<Infantry>(RobotInfo{1, "重复编号", Team::Blue}));
    } catch (const std::exception& e) {
        std::cout << "登记失败：" << e.what() << '\n';
    }

    // === 处理比赛事件 ===
    std::cout << "\n=== 处理比赛事件 ===\n";
    const std::vector<MatchEvent> events = {
        {EventType::Damage, 1, 35},
        {EventType::Damage, 2, 50},
        {EventType::Supply, 1, 0},
        {EventType::Supply, 2, 0},
        {EventType::Supply, 4, 0},
        {EventType::Damage, 3, 200},
        {EventType::Supply, 3, 0},
        {EventType::Damage, 99, 10},
        {EventType::Damage, 1, -5},
        {EventType::Supply, 1, 0},
        {EventType::Supply, 1, 0},
    };

    for (std::size_t i = 0; i < events.size(); ++i) {
        const MatchEvent& ev = events[i];
        std::cout << "事件 " << (i + 1) << "：";

        // 先输出事件描述文字
        if (ev.type == EventType::Damage) {
            std::cout << "机器人 " << ev.robotId << " 受到 " << ev.value << " 点伤害 -> ";
        } else {
            std::cout << "机器人 " << ev.robotId << " 接收补给 -> ";
        }

        try {
            int change = system.processEvent(ev);        // 失败会抛异常
            Robot* r = system.findRobot(ev.robotId);     // 成功后再查一次，拿当前血量

            if (ev.type == EventType::Damage) {
                std::cout << "成功，实际扣除 " << change << " 点";
            } else {
                std::cout << "成功，实际恢复 " << change << " 点";
            }
            std::cout << "，生命值：" << r->hp() << '/' << r->maxHp();
            if (!r->isAlive()) {
                std::cout << "，已退场";
            }
            std::cout << '\n';
        } catch (const std::exception& e) {
            std::cout << "失败：" << e.what() << '\n';
        }
    }

    // === 最终状态 ===
    std::cout << "\n=== 最终状态 ===\n";
    system.printStatus();
    std::cout << "成功处理事件数：" << system.successfulEvents() << '\n';

    return 0;
}
