//
//  Copyright (c) 2018 Hirotaka Yuno <create.future.technology@gmail.com>.  All right reserved.
//

#ifndef POWER_CTRL_H_
#define POWER_CTRL_H_

class PowerCtrl {
 public:
  static PowerCtrl& GetInstance();
  void ClearWUF();
  void EnterStandbyMode();

 private:
  PowerCtrl() = default;  // singleton
  virtual ~PowerCtrl() = default;
};

#endif  // POWER_CTRL_H_
