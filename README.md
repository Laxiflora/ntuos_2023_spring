> 免責聲明
> 我的實作方法並不合spec的規範，以下原文，github抄人請注意:
>> You should separate the child process and the parent process outputs with a blank line. You can print the blank line in either the child process or the parent process.
     However, if you do not use a pipe to receive dir num and file num and print out the result in the parent process, you will be regarded as CHEATING and get 0 points in this MP.



# MP0
spec詳見MP0.pdf

## 實作流程
透過觀察可以發現，其實作業給的xv6 kernel中的"count" user application，已經幫忙做完了作業需求的80%，剩下的就只是處理他的字串而已

這裡可以選擇
1. copy他的程式碼到mp0.c裡面改動，這種作法要求要能看懂並修改count.c
2. 直接fork child用exec執行這個程式，再由parent process處理輸出

作業規範上必須選1，但我感覺2很有趣(?)，所以在沒作業壓力下就直接做2.了。

pipe具備有「輸入端開著他的fd時，就算pipe清空讀取端也不會貿然判定已經不會有讀入」的特性，**所以這裡其實可以不用wait(pid)，不等除了可以達到真正的併行處理以外，對於pipe輸入過大導致pipe被塞滿讓程式直接卡死的狀況(child exit不了，parent傻傻地等)也不會發生**$^{[1]}$

合理推斷2.的做法在輸入夠大的前提下應該可以跑得比1.還快?

透過2.的作法，最難的部分就是針對count的輸出做字串處理了，這邊留給大家直接看扣XD，其實沒有很特別的難點，發揮創意即可。

---

$^{[1]}$在xv6裡面不確定是否system call與ubuntu等linux-like的OS有所不同，根據`man 7 pipe`  中"I/O on pipes and FIFOs"所言:

> If a process attempts to read from an empty pipe, then read(2) will block until data is available. If a process attempts to write to a full pipe (see below), then writ (2) blocks until sufficient data has been read from the pipe to allow the write to complete. Nonblocking I/O is possible by using the fcntl(2) F_SETFL operation to enable the O_NONBLOCK open file status flag.

前面的推測應該成立(事實上在寫交大NP作業時也是如此)，但在這裡如果沒有wait child的話，會甚麼都讀取不到(read return 0，非-1)