#!/bin/bash

#tools フォルダ下で実行
cd `dirname $0`
# コンパイル
g++ -std=c++17 -O2 -Wall -Wextra ../main.cpp
g++ -std=c++17 -O2 output_checker.cpp -o output_checker.out
#[st, en] のseed のファイルを処理する．procsはプロセス数，print_errorはxargs のエラー出力表示
st=1
en=100
procs=8
parameter=0
# parameter
st1=100
en1=10
st2=100
en2=10
# インタラクティブ処理の関数
# ビジュアライザに通して，スコアを標準出力として取得する関数
f1(){
  ./a.out < in/input$1.txt > out/$1.txt 2>> error.txt
}
f2(){
   ./output_checker.out in/input$1.txt out/$1.txt >> score.txt 2>error.txt
}
f3(){
  ./a.out $st1 $en1 $st2 $en2 < in/input$1.txt > out/$1.txt 2>/dev/null
}
# xargs で関数使うための処理
export -f f1
export -f f2
export -f f3
#以前の処理を削除
rm -f score.txt
rm -f error.txt
rm -rf out
mkdir out

# option

usage(){
  cat <<EOM
使い方：
  -s : 開始 seed
  -e : 終了 seed
  -P : プロセス数
  -p : パラメータ調整？
ただし，開始 seed から終了 seed までの入力ファイルは tools/in 下に置いておいてください．
EOM

  exit 2
}

while getopts "s:e:P:p:A:B:C:D" optKey; do
  case "$optKey" in
    s)
      st=${OPTARG}
      ;;
    e)
      en=${OPTARG}
      ;;
    P)
      procs=${OPTARG}
      ;;
    p)
      parameter=1
      ;;
    A)
      st1=${OPTARG}
      ;;
    B)
      en1=${OPTARG}
      ;;
    C)
      st2=${OPTARG}
      ;;
    D)
      en2=${OPTARG}
      ;;
    '-h' | '--help' | *)
      usage
      ;;
  esac
done

# 並列処理
if [ $parameter = 1 ];
then
  seq -f '%04g' $st $en | xargs -n1 -P$procs -I{} bash -c "f3 {}"
  seq -f '%04g' $st $en | xargs -n1 -P1 -I{} bash -c "f2 {}"
else 
  seq -f '%04g' $st $en | xargs -t -n1 -P$procs -I{} bash -c "f1 {}"
  seq -f '%04g' $st $en | xargs -t -n1 -P1 -I{} bash -c "f2 {}"
fi
# score.txt に書き込まれたスコアの計算
python3 evaluate.py