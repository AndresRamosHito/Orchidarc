library ctime;

import 'dart:core';

class MSTime {
  int timestamp;
  MSTime({this.timestamp});
  // return seconds
  double getSeconds() {
    return (DateTime.now().millisecondsSinceEpoch - timestamp) / 1000;
  }

  double getMinutes() {
    return getSeconds() / 60;
  }

  double getHours() {
    return getMinutes() / 60;
  }

  double getDay() {
    return getHours() / 24;
  }

  double getWeek() {
    return getDay() / 7;
  }

  String getFormattedDate() {
    var date = DateTime.fromMicrosecondsSinceEpoch(timestamp * 1000);
    if (getSeconds() < 60) {
      return "${getSeconds().toInt().toString()} ";
    }
    if (getMinutes() < 60) {
      return "${getMinutes().toInt().toString()} min ago";
    }
    if (getHours() < 24) {
      return '${getHours().toInt().toString()} hrs ago';
    }
    if (getDay() < 7) {
      return '${getDay().toInt().toString()} day ago';
    }
    if (getDay() > 7 && getDay() < 30) {
      return '${getWeek().toInt().toString()} week ago';
    }
    return date.toString();
  }
}
