#ifndef FSTEX_ROBOT_H
#define FSTEX_ROBOT_H

#include "Types.h"

#include <string>

namespace rm {

// Robot 抽象基类：所有机器人的统一接口
class Robot {
public:
    Robot(RobotInfo info, int maxHp);   // 构造函数：传入基础信息和最大生命值
    virtual ~Robot() = default;         // 虚析构：保证子类对象能正确销毁

    const RobotInfo& info() const;      // 查询编号/名称/阵营（只读，不改对象）
    int hp() const;                     // 查询当前生命值（只读）
    int maxHp() const;                  // 查询最大生命值（只读）
    bool isAlive() const;               // 是否存活，hp > 0 为 true（只读）
    int takeDamage(int damage);         // 扣血，返回实际扣除量（会改 hp_）

    // 纯虚函数：由派生类决定补给效果和类型名
    virtual int supply() = 0;                 // 补给，返回实际恢复量（子类实现）
    virtual std::string typeName() const = 0; // 类型名（子类实现，只读）

protected:
    // 派生类复用规则的辅助函数
    int recover(int amount);    // 恢复生命值，返回实际恢复量，不超过 maxHp
    void requireAlive() const;  // 已退场时抛 std::logic_error

private:
    RobotInfo info_;  // 编号/名称/阵营
    int hp_;          // 当前生命值
    int maxHp_;       // 最大生命值
};

// 步兵：最大生命值 100，单次地面补给恢复 20
class Infantry : public Robot {
public:
    explicit Infantry(RobotInfo info);      // 构造，最大生命值 100
    int supply() override;                  // 补给，恢复 20
    std::string typeName() const override;  // 返回 "步兵"
};

// 英雄：最大生命值 150，单次地面补给恢复 30
class Hero : public Robot {
public:
    explicit Hero(RobotInfo info);          // 构造，最大生命值 150
    int supply() override;                  // 补给，恢复 30
    std::string typeName() const override;  // 返回 "英雄"
};

// 哨兵：最大生命值 120，单次地面补给恢复 10
class Sentry : public Robot {
public:
    explicit Sentry(RobotInfo info);        // 构造，最大生命值 120
    int supply() override;                  // 补给，恢复 10
    std::string typeName() const override;  // 返回 "哨兵"
};

// 空中机器人：最大生命值 80，不支持地面补给
class AerialRobot : public Robot {
public:
    explicit AerialRobot(RobotInfo info);   // 构造，最大生命值 80
    int supply() override;                  // 地面补给失败，抛异常
    std::string typeName() const override;  // 返回 "空中机器人"
};

} // namespace rm

#endif // FSTEX_ROBOT_H
