#ifndef FSTEX_TYPES_H
#define FSTEX_TYPES_H

#include <string>

namespace rm {

// Team 枚举：阵营
enum class Team {
    Red,  // 红方
    Blue  // 蓝方
};

// RobotInfo 结构体：编号、名称、阵营
struct RobotInfo {
    int id;            // 编号，全系统唯一
    std::string name;  // 名称，不能为空
    Team team;         // 阵营
};

// EventType 枚举：事件类型
enum class EventType {
    Damage,  // 伤害事件，value 为伤害值（必须 > 0）
    Supply   // 补给事件，value 被忽略（填 0）
};

// MatchEvent 结构体：事件类型、机器人编号、数值
struct MatchEvent {
    EventType type;  // 事件类型
    int robotId;     // 目标机器人编号
    int value;       // 伤害时为伤害值，补给时忽略（填 0）
};

} // namespace rm

#endif // FSTEX_TYPES_H
