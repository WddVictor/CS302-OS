#ifndef JOB_H
#define JOB_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <signal.h>
#include <sys/types.h>

#define	FIFO "/tmp/schedule.fifo"

#ifndef DEBUG
 #define DEBUG
#endif

#undef DEBUG

#define BUFLEN 100
#define GLOBALFILE "screendump"
//闁煎拵鎰嗚瑢锝柤锝甸柤锝宠闁硷姜閬掔憵鈹婚畻妤⑩敂铚婏舰璜★舰璨庢▕鏃柤锝ラ柤锝ㄨ璜★舰铚婏舰闁硷江闁硷剑闁硷建闁硷建璜★舰铚婏舰闁硷渐楂炲锝测埇鈹
enum jobstate 
{
  READY, RUNNING, DONE
};
//闁煎拵鎰嗚瑢锝柤锝甸柤鍜忔忛柤鍕熻壆闁肩憵锝块柤锝ㄩ柤锝ヨ锝㈣矌妯ｅ垘椹燂蕉楂达讲闁硷将璜★舰铚婏舰榄庘埖妞㈤锝查柤锝锝㈣湂锝㈤亽鎮熷⒕闁兼粨瀹嶉柤锝抽畻妤⑩敂铚婏舰闄屽壈妗侀柤锝╄饯
enum cmdtype 
{
  ENQ = -1, DEQ = -2, STAT = -3
};

/* this is data passed in fifo */
//闁硷匠璀撮柤锝亽妾庯舰姊э舰楂达讲闁奸柤鍜忔忛柤鍕熻壆楂毠锝╅畺锝块畺锝
struct jobcmd {
  enum	cmdtype type;//闁硷匠璀撮柤锝亽鐟氬拸鎬忛柤鍕熻壆闁肩憵锝块柤锝ㄩ柤锝
  int	argnum;//闄屽壈鈹柤锝㈣煇瀹氀婇柤锝块柤锝ㄩ柤锝
  int	owner;//闁硷匠璀撮柤锝亽妾庯舰鐟氬瓋wner
  int	defpri;//闁煎笡鍥伴柤闁硷涧闁硷将闁兼噯鈹ら柤锠栨毊鎰
  char	data[BUFLEN];//闁硷舰锜愬憟鏃柤锝婚柤鐟氾娇闁硷建闁硷渐
};

#define DATALEN sizeof(struct jobcmd)
#define error_sys printf

struct jobinfo {
  int    jid;                 /* 闁硷匠璀撮柤锝亽鏅痙 */
  int    pid;                 /* 楂锝堕瓗鈭敂id */
  char** cmdarg;              /* 闁硷桨楫楁ア鈹撮柤锝ㄩ亽鐟氬箘鍜忔忛柤鍕熻壆閬栭泲鐟氣敍闁硷浇闄屽壈鈹柤锝㈣煇绋歨e command & args to execute */
  int    defpri;              /* 闁煎笡鍥伴柤闁硷涧闁硷将闁兼噯鈹ら柤闁奸伣   default priority */
  int    curpri;              /* 閬掓偀澧鹃柤鏅佲敤閬掔憵骞勨攦闁兼噯鈹ら柤闁奸伣 current priority */
  int    ownerid;             /* 闁硷匠璀撮柤锝亽鐟氣攦閬掔憵鈹冮柤锝ㄩ柤锝柤锝絠d the job owner id */
  int    wait_time;           /* 闁硷江闁硷椒閬掔憵鈹傞畻鐏橈姜甯濇剢闁硷将闁兼垔鈹撮柤锝伴柤锝ㄩ亽鐟氣攤楫楃仒锝牑锝㈢憵骞勨攼铚婏饯锠栨厱宄噒he time job in waitqueue */
  time_t create_time;         /* 楫楅Υ锝洪Υ鍒庨伣妤⑩敾璀撮柤锝亽妾庯舰鐟氬箘鈹愯湂锝よ爾鎱曞硣the time job create */
  int    run_time;            /* 闁硷匠璀撮柤锝亽鐟氣敁闁硷剑闁硷建闁硷建閬掔憵骞勨攼铚婏饯锠栨厱宄噒he time job running */
  enum   jobstate state;      /* 闁硷匠璀撮柤锝亽鐟氣敾楫楁ア鈹旇湂锝ob state */
};

struct waitqueue {            /* 闁硷剑闂冿溅闁硷涧闆嬫獛锝㈢憵骞勬毊锝虹仒澧鹃珨 double link list */
  struct waitqueue *next;    //闁硷涧闁艰嫇鈹侀仏瑗嶃倛妞㈤亽鐟氣攤楫楃仒锝畾鈹昏闁硷姜閬
  struct jobinfo *job;       //閬掓偀澧鹃柤婊撳畭閬掔憵鈹傞畻鐏橈姜瀹氣敾璀撮柤锝亽妾庯舰鐟氬箘鈹撮柤鎳嗏敜铦
};

void schedule();//閬掓ⅶ锝㈤锝查柤璨庢⊕锝夐柤锝㈣煇
void sig_handler(int sig, siginfo_t *info, void *notused);//闁硷建闁兼噲锝芥珌鎲猴胶鐏橈建闁肩瀹
int  allocjid();//闇嶏蒋闁硷桨闁兼噧宄囬柤锝宠闁硷姜閬掓櫙d
void do_enq(struct jobinfo *newjob, struct jobcmd enqcmd);//闁艰眻骞冩剢闁硷将璨庢⊕锝夐柤锝㈣煇
void do_deq(struct jobcmd deqcmd);//榄庘埖妞㈤锝查柤锝矌妯掞綁闁硷舰锜
void do_stat();//闁硷涧闁硷江闁硷舰璎岋疆闁硷匠璀撮柤锝亽鐟氣敾楫楁ア鈹旇湂锝
void updateall();//瑗嶃倝鎬忛柤锝ㄩ柤鑻撯敇楂磋娊鈹冮柤锝ㄩ柤锝宠闁硷姜閬掔憵鈹撮柤鎳嗏敜铦
struct waitqueue* jobselect();//閬掔憵鈹傞畻鐏橈姜甯濇剢闁硷将闁兼垔鈹撮柤锝伴柤锝ㄩ柤锝╅闁硷江锜讹浇闁硷匠璀撮柤锝亽
void jobswitch();//闁硷匠璀撮柤锝亽鐟氣敾楂嫍锝﹂仏

#endif
