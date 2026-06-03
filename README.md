This project has been created as part of the 42 curriculum by mbouyer.


# Description

Codexion is a multithreaded simulation of concurrent coders sharing USB dongles. It is inspired by the Dining Philosophers problem.

Codexion is a concurrent programming project that simulates a group of coders working in a shared office. Each coder needs two USB dongles to compile their code. Since dongles are shared between coders, they have to coordinate access to avoid deadlocks and starvations.

This projet demonstrates:
- POSIX thread management (pthread)
- Mutex and condition variable synchronisation
- Priority-based scheduling (FIFO and EDF)
- Deadlock prevention via ordered ressource acquisition
- A custom heap priority queue for a dongle arbitration

Each coder sits at a round table. Between every two adjacent coder lies a shared USB dongle. To compile, a coder must pick up **both** their left and right dongle. After compiling, they debug and refactor before compiling again.

If a coder goes too long without compiling, they **burn out** and the simulation stops.


# Instructions

## Usage
bash / Clone and build / git clone <repo_url> / cd codexion

* compiled with cc -Wall - Wextra -Werror - pthread
* make        # build
* make clean  # remove object files
* make fclean # remove objects + binary
* make re     # full rebuild

## Arguments

| Argument | Description |
|---|---|
| `nb_of_coders` | Number of coders (>= 1) |
| `t_burnout` | Time in ms befor a coder burns out without compiling |
| `t_compile` | Time in ms to compile |
| `t_debug` | Time in ms to debug |
| `t_refactor` | Time in ms to refactor |
| `nb_of_compiles` | Number of compiles required per coder to finish |
| `cooldown` | Cooldown time in ms between dongle uses |
| `scheduler` | Dongle arbitration policy: `fifo` or `edf` |

All time values are in **milliseconds**.

## Schedulers

### `fifo`: First in, First Out
Coders are granted access to a dongle in the order they requested it. Uses the request timestamp as priority.

### `edf`: Earliest Deadline First
The coder whose burnout dealine is closest get priority. Prevent the most at-risk from starving.

The scheduler controls the order of the **mini-heap priority queue** used inside each dongle.

## Examples

5 coders, 800ms burnout, 200ms compile, each must compile 3 times, FIFO scheduling
./codexion 5 800 200 200 200 3 100 fifo


# Blocking cases handled :

### Deadlock prevention (Coffman's conditions)
The classic deadlock scenario in the Dining Philosophers problem arises from circular wait: each coder holds one resource and waits for the next, forming a cycle. Codexion breaks this by enforcing **resource ordering**: each coder always acquires the lower-indexed dongle first. Since no circular dependency can form, deadlock is impossible. This directly eliminates one of Coffman's four conditions for deadlock.

### Starvation prevention
With `fifo` scheduling, each dongle maintains a priority queue ordered by arrival time — the first coder to request a dongle is the first to receive it, guaranteeing forward progress for every coder. With `edf` scheduling, the coder whose burnout deadline is earliest is served first, minimizing the risk that any coder exceeds its deadline. Tie-breaking uses coder ID to ensure a fully deterministic policy in edge cases.

### Cooldown handling
After releasing a dongle, it becomes unavailable for `dongle_cooldown` milliseconds. The dongle tracks the timestamp of its next availability (`available_at`). Waiting coders use `pthread_cond_timedwait` to sleep until the cooldown expires, avoiding busy-waiting while maintaining precise timing.

### Precise burnout detection
A dedicated **monitor thread** polls all coder deadlines every 1 ms. When a coder's deadline is crossed, the monitor logs the burnout immediately and sets the global stop flag. This ensures the burnout message appears within the required 10 ms window of the actual burnout time.

### Log serialization
All `printf` calls are protected by a dedicated `log_mutex`. Before printing, the log functions also check the stop flag (under `stop_mutex`) to suppress spurious messages after a burnout or successful completion.

## Thread synchronization mechanisms

### Primitives used

**`pthread_mutex_t`** is used in three contexts:
- `dongle.mutex`: protects each dongle's state (available, cooldown, wait queue)
- `sim.stop_mutex`: protects the global `stopped` flag, shared between all coder threads and the monitor
- `sim.log_mutex`: serializes all output so messages never interleave

**`pthread_cond_t`** (`dongle.cond`) is associated with each dongle mutex. When a dongle is unavailable (busy or in cooldown), requesting coders call `pthread_cond_timedwait` to sleep efficiently until the condition may have changed. When a dongle is released, `pthread_cond_broadcast` wakes all waiters so the scheduler (priority queue) can grant access to the rightful next coder.

**Priority queue (min-heap)** — implemented from scratch without standard library helpers — maintains the ordered wait list for each dongle. For FIFO, priority is the arrival timestamp. For EDF, priority is `last_compile_start + time_to_burnout`. The heap guarantees O(log n) push/pop with correct ordering.

### Race condition prevention

When a coder is granted a dongle, it checks the stop flag while still holding the dongle mutex, then re-checks after acquiring both dongles before logging "is compiling". This two-phase check prevents a coder from starting a compile after a burnout has been declared.

The monitor thread never holds any dongle mutex — it only reads coder deadlines (written atomically by the owning coder thread) and writes the `stopped` flag under `stop_mutex`. This avoids priority inversion between the monitor and coder threads.

### Thread-safe communication between coders and monitor

Coders update their `last_compile_start` and `deadline` fields immediately before logging "is compiling", while holding no shared lock. The monitor reads these fields without locking, which is safe because each field is owned by a single coder thread. The only shared write path is the `stopped` flag, always accessed under `stop_mutex`.


# Resources
- W3 schools : as I am programming in Python, I forgot C programming language. W3 schools is full of exercises where we can practise a programming language. It was a good way to remind myself C language. 
- A website explaining the dining philosophers:
https://medium.com/@yassinx4002/dining-philosophers-in-c-from-theory-to-practice-28582180aa37


# AI usage

I have used deepseek and chatgpt to help me design some exercises. With those exercises, I have increased my knowledge before starting codexion project.

Claude AI helps me to write the readme file.