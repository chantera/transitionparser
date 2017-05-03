//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_APP_H_
#define COORDPARSER_APP_H_

namespace coordparser {

class App {
 public:
  App();

  void train();

 private:
  void initialize();
};

}  // namespace coordparser

#endif  // COORDPARSER_APP_H_
