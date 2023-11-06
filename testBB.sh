#!/usr/bin/env bash
testcase=$1
rm -f DEBUG RESULT 
if [[ -z "$testcase" ]]; then
  testcase=1
fi 

testcnt=1
output_fn="./output.txt"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
if [[ -d /usercode ]]; then
  cd /usercode || exit 33
else
  cd "$DIR/.." || exit 33
fi

rm -f DEBUG RESULT output.txt STDERR

# Function to be called upon exit
function on_exit() {
    if grep -E "^p" RESULT > /dev/null; then
      echo "Exiting tester." >> DEBUG
    else
      showOutput
      echo "Error occurred, exiting tester ." >> DEBUG
    fi
    if ! pgrep -f "coding_rooms_unit_tests.sh"; then  # if not running under this script then print results 
      cat DEBUG RESULT
    fi
}

# Trap the exit signal to call on_exit function
trap on_exit EXIT

function standardExec(){
  ./a.out "$@" > output.txt < INPUT 2>> DEBUG
}

function testoutput(){
  EXPECTED="$1"
  grep_opts="$2"

  while IFS=" " read -r line; do
    if cat output.txt | tr -d " " | grep ${grep_opts} "${line// /}" > /dev/null ; then
      continue
    else
      log_neg "\t\033[38;5;3mMISSING '${line}' in output. \033[0m \n"  >> DEBUG
      return 1 ## BASH thinks this is False
    fi
  done <<< $EXPECTED

  return 0 ## BASH is fun, this is True
}


function showOutput() {

	if [[ -f INPUT ]]; then
	  echo "---- std input ---- " >> DEBUG
	  cat INPUT >> DEBUG
	fi
	if [[ -f raw_output.txt ]]; then
    echo -e "\n---- std out  ---- " >> DEBUG
    cat raw_output.txt >> DEBUG
    printf "\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n" >> DEBUG
  elif [[ -f output.txt ]]; then
    echo -e "\n---- std out   ---- " >> DEBUG
    cat output.txt >> DEBUG
    printf "\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n" >> DEBUG
  fi

}

function compile(){

  rm -f a.out
  binary_fn="./a.out"

  foundcpp=false

  for arg in "$@"; do
    if [[ "$arg" == *.cpp ]]; then
        foundcpp=true
        break
    fi
  done

  if ${foundcpp}; then
    g++ "$@" -Wall -Werror -g -o ${binary_fn} >> DEBUG 2>&1
  else
    gcc "$@" -Wall -Werror -g -o ${binary_fn} >> DEBUG 2>&1
  fi
  compile_ret=$?

  if [[ $compile_ret -ne 0 ]]; then
      printf "\t\033[38;5;3mFAILED to compile " >> DEBUG
      echo "np" > RESULT
      exit 44
  fi
}

log_pos() {
    msg="$1"
    printf "\t${testcnt}. \033[38;5;10m✔ ️$msg\033[0m\n" >> DEBUG
    testcnt=$(( testcnt + 1 ))
}
log_neg() {
    msg="$1"
    printf "\n\033[38;5;3m$msg\033[0m\n" >> DEBUG
    echo "np" > RESULT
}

###########################################################################################
#  END FUNCTIONS SECTION 
###########################################################################################

if [[ -f song-aa ]]; then 
    cat song-aa song-ab song-ac > songs.txt 
elif [[ -f songs2.5k ]]; then 
    cp songs2.5k.txt songs.txt 
fi 

compile "main.c"

# source code tests
grep_opts="-i "
binary_fn="./a.out" 

#ls -ls ${binary_fn} >> DEBUG 
#if [[ "$testcase" -eq 1 ]]; then
 if grep -n 'genre.*artist' songs.txt > /dev/null;  then
    genrePos=$(grep -n 'genre.*artist' songs.txt | cut -d ":" -f1 )
    sed "${genrePos}d" songs.txt > /tmp/songs_noheader.txt
else 
    cp songs.txt /tmp/songs_noheader.txt
fi 
songcnt=$(cat /tmp/songs_noheader.txt | wc -l)

if [[ $testcase -eq 1 ]]; then 
   

    if [[ "$songcnt" -eq 2448 ]]; then
        songIndex=2000    
    else
        songIndex=$(( songcnt - 10 ))
    fi 

    songName=$( cat /tmp/songs_noheader.txt | sed -n "$(( songIndex + 1 ))p" | cut -d $'\t' -f3)

    printf "q\n" > INPUT

    printf "number of songs $songcnt, songIndex=${songIndex}, songName=${songName}\n" >> DEBUG 

    gdb ${binary_fn}  --batch -ex "b printSongList" -ex "set listsize 20" -ex "set disable-randomization off" -ex "run < INPUT"  -ex "finish" -ex 'printf "songList.songs['${songIndex}'].title = %s\n",songList.songs['${songIndex}'].title'  > ./output.txt 2>> DEBUG
    
    
    EXPECTED="songList.songs\[${songIndex}\].title = ${songName}"

    if testoutput "$EXPECTED"; then
        echo 'p' > RESULT
        log_pos "PASSED, passed all tests"
    else
        printf '\033[38;5;9mCould not verify that songList was an Array List. Is the function printSongList being called from main? Is songList defined in main?  \n\033[0m' >> DEBUG        
        printf 'Test with this command locally to diagnose the issue: \n' >> DEBUG 
        printf '\ngcc main.c -g -o %s && gdb %s -ex "b printSongList" \\ \n' "${binary_fn}" "${binary_fn}"
        printf '\t-ex "set disable-randomization off" -ex "run <<< '"'"'q\\n'"'"'" -ex "finish" \\ \n'
        printf '\t-ex '"'"'printf "song['${songIndex}'] = %%s\\n",songList.songs['${songIndex}'].title'"'"' \n' >> DEBUG
        
        echo 'np' > RESULT
        exit 95
    fi
elif [[ $testcase -eq 2 ]]; then 
    
    numPops=$(cat songs.txt | wc -l)
    if [[ $numPops -lt 120 ]]; then 
        printf "Error, songs.txt must have at least 120 Pop songs in it\n"
        exit 128
    fi 

    printf "4
n
n
n
n
n
p
p
p
p
p
q
" > INPUT

    standardExec 
    
    grep_opts="-i "
    firstpops=$(head -10 /tmp/songs_noheader.txt| grep -E "^Pop" | wc -l)

    popcnt=$(cat output.txt | tr -d " " | grep -E "[0-9]+\.Pop" | wc -l)
    
    printf "popcnt=${popcnt}\n" >> DEBUG 

    if [[ "$popcnt" -ne $(( 110 + firstpops )) ]]; then
        printf "\033[38;5;1mSet to Pop, then 5n's followed by 5p's expecting 110 (initial 10 Pops on selecting 4, 50 Pops up, and 50 Pops down) actual Pops = '${popcnt}'\033[0m\n"
        echo "np" > RESULT 
        exit 22
    fi 

    EXPECTED="Billie Eilish - wish you were gay
    Billie Eilish - idontwannabeyouanymor
    Ariana Grande - make up 
    "

    if testoutput "$EXPECTED"; then
        echo 'p' > RESULT
        log_pos "PASSED, passed all tests"
    else
        printf "in the fail part '$line' \n" >> DEBUG 
        printf "${line// /} search in $(wc output2.txt)\n" >> DEBUG 
        log_neg "\033[38;5;1mFAILED test '$line' as input\033[0m\n" >> DEBUG    
        echo "np" > RESULT 
        exit 122
    fi
else
  echo "np" > RESULT
  exit 33
fi






