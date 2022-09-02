// See LICENSE for license details.

#include "pk.h"
#include "atomic.h"
#include "frontend.h"
#include "sbi.h"
#include "mcall.h"
#include "syscall.h"
#include <stdint.h>

long frontend_syscall(long n, long a0, long a1, long a2, long a3, long a4, long a5, long a6)
{
  static volatile uint64_t magic_mem[8];

  static spinlock_t lock = SPINLOCK_INIT;
  spinlock_lock(&lock);

  magic_mem[0] = n;
  magic_mem[1] = a0;
  magic_mem[2] = a1;
  magic_mem[3] = a2;
  magic_mem[4] = a3;
  magic_mem[5] = a4;
  magic_mem[6] = a5;
  magic_mem[7] = a6;

  do_mcall(MCALL_HTIF_SYSCALL, magic_mem);

  long ret = magic_mem[0];

  spinlock_unlock(&lock);
  return ret;
}

void shutdown(int code)
{
  frontend_syscall(SYS_exit, code, 0, 0, 0, 0, 0, 0);
  while (1);
}

void copy_stat(struct stat* dest, struct frontend_stat* src)
{
  dest->st_dev = src->dev;
  dest->st_ino = src->ino;
  dest->st_mode = src->mode;
  dest->st_nlink = src->nlink;
  dest->st_uid = src->uid;
  dest->st_gid = src->gid;
  dest->st_rdev = src->rdev;
  dest->st_size = src->size;
  dest->st_blksize = src->blksize;
  dest->st_blocks = src->blocks;
  dest->st_atime = src->atime;
  dest->st_mtime = src->mtime;
  dest->st_ctime = src->ctime;
}
