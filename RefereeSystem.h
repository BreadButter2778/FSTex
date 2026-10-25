#ifndef FSTEX_REFEREE_SYSTEM_H
#define FSTEX_REFEREE_SYSTEM_H

#include "Robot.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace rm {

// 裁判系统：登记机器人、处理事件、维护状态并输出
class RefereeSystem {
public:
    // 登记机器人：空指针/重复编号/非法信息时抛 std::invalid_argument
    void registerRobot(std::unique_ptr<Robot> robot);

    // 按编号查找机器人，找不到返回 nullptr（指针只用于访问，不能释放）
    Robot* findRobot(int id);

    // 处理一条事件，返回实际扣除/恢复的生命值，失败时抛异常
    int processEvent(const MatchEvent& event);

    // 已成功处理的事件数（只读）
    std::size_t successfulEvents() const;

    // 按登记顺序输出所有机器人状态（只读）
    void printStatus() const;

private:
    std::vector<std::unique_ptr<Robot>> robots_;  // 按登记顺序持有所有机器人
    std::size_t successfulEvents_ = 0;            // 成功事件计数
};

} // namespace rm

#endif // FSTEX_REFEREE_SYSTEM_H
