/**
 * Generated by pabble-mpi v1.2.1
 * Date: Mon Oct 27 12:06:06 2014
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <scribble/pabble.h>

/*** MPI tags ***/
#define Init 0
#define Data 1
#define Finish 2
#define P_RANK(x) (0+((x)-(1)))

extern meta_t meta;


int main(int argc, char *argv[])
{
  double ts_overall=0.0, ts_protocol=0.0;
  MPI_Init(&argc, &argv);
  ts_overall = MPI_Wtime();
  meta.comm = MPI_COMM_WORLD;
  MPI_Comm_rank(MPI_COMM_WORLD, &meta.pid);
  MPI_Comm_size(MPI_COMM_WORLD, &meta.nprocs);
  MPI_Group world_grp;
  MPI_Comm_group(MPI_COMM_WORLD, &world_grp);
  /** Declarations (Protocol Alltoall) **/
  int N = ((int) pow((double)meta.nprocs, 1/1));
  MPI_Barrier(MPI_COMM_WORLD); /* Protocol begin */
#pragma pabble type T
  typedef void T;
  MPI_Datatype MPI_T = MPI_BYTE;
  T *bufData0_r;
  MPI_Request req0_r; MPI_Status stat0_r;
  T *bufData0_s;
  MPI_Request req0_s; MPI_Status stat0_s;
  ts_protocol=MPI_Wtime();
#pragma pabble Init
#pragma pabble recur Loop
  while (1/*Loop*/) {
    bufData0_r = calloc(meta.buflen(Data), sizeof(T));
    MPI_Irecv(bufData0_r, meta.buflen(Data), MPI_T, /*w/o cond.*/__All, Data, meta.comm, &req0_r);
    MPI_Wait(&req0_r, &stat0_r);
    pabble_recvq_enqueue(Data, bufData0_r);
#pragma pabble Data
#pragma pabble Data
    bufData0_s = pabble_sendq_dequeue();
    MPI_Isend(bufData0_s, meta.buflen(Data), MPI_T, /*w/o cond.*/__All, Data, meta.comm, &req0_s);
    MPI_Wait(&req0_s, &stat0_s);
    free(bufData0_s);
    continue;
   break;
  }
#pragma pabble Finish
  MPI_Barrier(MPI_COMM_WORLD); /* Protocol end */
  ts_protocol=MPI_Wtime()-ts_protocol;
  ts_overall=MPI_Wtime()-ts_overall;
  MPI_Finalize();
  if (meta.pid==0) fprintf(stderr, "Protocol=%fs Overall=%f\n", ts_protocol, ts_overall);
  return EXIT_SUCCESS;
}
