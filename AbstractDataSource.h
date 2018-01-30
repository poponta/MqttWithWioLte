//
//  Copyright (c) 2018 Hirotaka Yuno <create.future.technology@gmail.com>.  All right reserved.
//

#ifndef ABSTRACT_DATA_SOURCE_H_
#define ABSTRACT_DATA_SOURCE_H_

typedef struct st_CollectedData {
  double distance;
}st_CollectedData;

class AbstractDataSource {
 public:
  AbstractDataSource(){};
  virtual ~AbstractDataSource(){};
  AbstractDataSource(const AbstractDataSource&) = delete;
  AbstractDataSource& operator=(const AbstractDataSource&) = delete;
  
  virtual void Init() = 0;
  virtual void GetData(st_CollectedData *data) = 0;
};

#endif  // ABSTRACT_DATA_SOURCE_H_
