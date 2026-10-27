#include "Robot.h"

#include <algorithm>  // std::min
#include <stdexcept>  // std::logic_error
#include <utility>    // std::move

namespace rm {

// Robot 构造函数：初始化基础信息、当前生命值（满血）、最大生命值
Robot::Robot(RobotInfo info, int maxHp)
    : info_(std::move(info)), hp_(maxHp), maxHp_(maxHp) {}

// info()：返回基础信息（编号/名称/阵营）的只读引用
const RobotInfo& Robot::info() const {
    return info_;
}

// hp()：返回当前生命值（int 很小，直接按值返回）
int Robot::hp() const {
    return hp_;
}

// maxHp()：返回最大生命值
int Robot::maxHp() const {
    return maxHp_;
}

// isAlive()：生命值大于 0 即为存活，等于 0 视为退场
bool Robot::isAlive() const {
    return hp_ > 0;
}

// takeDamage()：扣血，返回实际扣除量
int Robot::takeDamage(int damage) {
    if (damage <= 0) {                          // 伤害值必须大于 0
        throw std::invalid_argument("伤害值必须大于 0");
    }
    requireAlive();                             // 已退场则抛 std::logic_error
    int actual = std::min(damage, hp_);         // 实际扣除量，最低扣到 0
    hp_ -= actual;                              // 真的扣血
    return actual;                              // 返回实际扣除量
}

// recover()：回血，返回实际恢复量（不超过最大生命值）
int Robot::recover(int amount) {
    int actual = std::min(amount, maxHp_ - hp_);  // 实际恢复量 = min(想回的量, 还能回的量)
    hp_ += actual;                                // 真的加血
    return actual;                                // 返回实际恢复量
}

// requireAlive()：检查是否退场，退场则抛 std::logic_error
void Robot::requireAlive() const {
    if (!isAlive()) {              // 生命值为 0 即退场
        throw std::logic_error("机器人已退场");
    }
}

// ===== Infantry（步兵）：最大生命值 100，补给恢复 20 =====

Infantry::Infantry(RobotInfo info)
    : Robot(std::move(info), 100) {}      // 调基类构造，固定最大生命值 100

int Infantry::supply() {                  // 补给恢复 20 点
    requireAlive();                       // 已退场则抛 std::logic_error
    return recover(20);                   // 复用基类回血逻辑，封顶在满血
}

std::string Infantry::typeName() const {  // 返回 "步兵"
    return "步兵";
}

// ===== Hero（英雄）：最大生命值 150，补给恢复 30 =====

Hero::Hero(RobotInfo info)
    : Robot(std::move(info), 150) {}      // 固定最大生命值 150

int Hero::supply() {                      // 补给恢复 30 点
    requireAlive();
    return recover(30);
}

std::string Hero::typeName() const {      // 返回 "英雄"
    return "英雄";
}

// ===== Sentry（哨兵）：最大生命值 120，补给恢复 10 =====

Sentry::Sentry(RobotInfo info)
    : Robot(std::move(info), 120) {}      // 固定最大生命值 120

int Sentry::supply() {                    // 补给恢复 10 点
    requireAlive();
    return recover(10);
}

std::string Sentry::typeName() const {    // 返回 "哨兵"
    return "哨兵";
}

// ===== AerialRobot（空中机器人）：最大生命值 80，不支持地面补给 =====

AerialRobot::AerialRobot(RobotInfo info)
    : Robot(std::move(info), 80) {}           // 固定最大生命值 80

int AerialRobot::supply() {                   // 地面补给：直接失败
    throw std::logic_error("空中机器人不支持地面补给");
}

std::string AerialRobot::typeName() const {   // 返回 "空中机器人"
    return "空中机器人";
}

} // namespace rm
