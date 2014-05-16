zeromq-learning
========

pub-sub model
--------

ソースコード
  * [src/zeromq-pub/zeromq-pub.cpp](src/zeromq-pub/zeromq-pub.cpp)
  * [src/zeromq-sub/zeromq-sub.cpp](src/zeromq-sub/zeromq-sub.cpp)

メモ
  * publishしたメッセージはすべてのsubscriberが受信するモデル
  * pub・subどちらもlisten側になることができる
  * pub・subどちら側から起動してもOK
  * pub・subのどちら側を再起動しても再接続される
  * 

push-pull model
--------

ソースコード
  * [src/zeromq-push/zeromq-push.cpp](src/zeromq-push/zeromq-push.cpp)
  * [src/zeromq-pull/zeromq-pull.cpp](src/zeromq-pull/zeromq-pull.cpp)

メモ
  * pushしたデータはどれか一つのpull側が受信するモデル
  * push・pullどちらもlisten側になることができる
    * push側がlistenしている場合は、誰かpullするまでzmq_send()はブロックする。
    * pull側がlistenしている場合は、pushしたメッセージは誰かがpullするまでキューイングされる
  * push・pullどちら側から起動してもOK
  * push・pullのどちら側を再起動しても再接続される

req-rep model
--------

ソースコード
  * [src/zeromq-req/zeromq-req.cpp](src/zeromq-req/zeromq-req.cpp)
  * [src/zeromq-rep/zeromq-rep.cpp](src/zeromq-rep/zeromq-rep.cpp)

