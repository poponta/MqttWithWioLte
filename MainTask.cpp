//
//  Copyright (c) 2018 Hirotaka Yuno <create.future.technology@gmail.com>.  All right reserved.
//

#include <ArduinoJson.h>
#include <RTClock.h>
#include <time.h>
#include "AbstractDataSource.h"
#include "CommonResource.h"
#include "DistanceSensor.h"
#include "EnergyManager.h"
#include "MainTask.h"
#include "NetworkManager.h"
#include "PowerCtrl.h"

// センサーのインスタンス
DistanceSensor dist_sen;

namespace {
  // センサーデータをJSONフォーマットに変換する関数
  void SerializeToJson(const st_CollectedData &data, char *json, const size_t &size) {
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["sensor"] = "dintance";
    root["data"] = data.distance;
    root.printTo(json, size);
  }
}

void MainTask::Init(NetworkManager *net_mgr, EnergyManager *eng_mgr) {
  net_mgr_ = net_mgr;
  eng_mgr_ = eng_mgr;
  state_ = kMeasurement;
  for (int i = 0; i < ConstantValue::DATA_SOURCE_NUM_MAX; i++) {
    data_source_[i] = NULL;
  }
  // インスタンス化したセンサーの代入
  data_source_[0] = &dist_sen;
}

// the setup routine runs once when you press reset:
void MainTask::setup() {
  struct tm current_time = {0};
  SerialUSB.println("MainTask Setup");
  // LTEモジュール起動
  net_mgr_->Init();
}

// the loop routine runs over and over again forever:
void MainTask::loop() 
{
  static st_CollectedData data = {0};  // センサーデータを入れる箱
  static char json[ConstantValue::JSON_MESSAGE_SIZE] = {0};  // JSONを入れる箱
  
  // State Machine
  switch (state_) {
    case kMeasurement:
      SerialUSB.println("State Change:kMeasurement");
      // 配下のセンサーの値を取得
      for (int i = 0; i < ConstantValue::DATA_SOURCE_NUM_MAX; i++) {
        data_source_[i]->GetData(&data);
        SerialUSB.print("Data SensorID ");
        SerialUSB.print(i);
        SerialUSB.print(" : ");
        SerialUSB.println(data.distance);
        if ((i < ConstantValue::DATA_SOURCE_NUM_MAX - 1) && (data_source_[i+1] == NULL)) {
          // 登録されている最後のセンサーの値を取得した
          break;
        }
      }

      // ステート遷移
      state_ = kPublish;
      break;
      
    case kPublish:
      // LTE網へアタッチし、PDPアクティベート
      net_mgr_->Activate();

      // MQTTブローカへ接続
      net_mgr_->MqttConnect();
      SerialUSB.println("State Change:kPublish");

      // センサーデータをJSONフォーマットに変換
      SerializeToJson(data, json, ConstantValue::JSON_MESSAGE_SIZE);
      SerialUSB.println(json);

      // ブローカーに送信
      if (net_mgr_->MqttPublish(json)) {
        SerialUSB.println("Publish Completed");
      } else {
        SerialUSB.println("Publish Failed");
      }

      // ステート遷移
      state_ = kExit;
      break;

    case kExit:
    default:
      SerialUSB.println("State Change:kExit");
      // タイマーをセットし、低消費電力モードへ遷移
      struct tm current_time = {0};
      time_t epoch = 0;
      if (net_mgr_->GetNtpTime(&current_time) == true) {  // NTPサーバから現在時刻を取得
        // 時刻の取得に成功
        SerialUSB.println("Get Time From NTP Server");
        SerialUSB.print("UTC:");
        SerialUSB.println(asctime(&current_time));
        epoch = mktime(&current_time);  // エポック秒に変換（足し算するため）
        epoch += ConstantValue::JAPAN_TIME_DIFF;  // 日本の時差を加算
        current_time = *gmtime(&epoch);
        eng_mgr_->SetCurrentTime(&current_time);  // 現在時刻をRTCモジュールに設定
        delay(1);  // RTCへの反映待ち
      }
      eng_mgr_->GetCurrentTime(&current_time);
      SerialUSB.print("Internal RTC:");
      SerialUSB.println(asctime(&current_time));
      epoch = mktime(&current_time);  // エポック秒に変換（足し算するため）
      epoch += ConstantValue::WAKE_UP_INTERVAL;  // 15分足す
      net_mgr_->Exit();  // LTE通信終了
      eng_mgr_->EnterStandbyMode(epoch);  // 15分後にアラームをセットし、スタンバイモードへ遷移
      // TUDO(YUNO): 25時を過ぎたら、次の起動は6時とする
      break;
  }
}
