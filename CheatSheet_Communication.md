# MPI CheatSheet

## Initialization and Finalization

#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Initialize the MPI environment
    // Your code here
    MPI_Finalize(); // Finalize the MPI environment
    return 0;
}

## Basic Point-to-Point Communication

**Sending Data**
int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);

- buf: Starting address of the send buffer.
- count: Number of elements in the send buffer.
- datatype: Data type of each send buffer element.
- dest: Rank of destination process.
- tag: Message tag.
- comm: Communicator.

**Receiving Data**
int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);

- buf: Starting address of the receive buffer.
- count: Maximum number of elements in the receive buffer.
- datatype: Data type of each receive buffer element.
- source: Rank of source process.
- tag: Message tag.
- comm: Communicator.
- status: Status object.

**Example: Point-to-Point Communication**
// Sender (Rank 0)
int data = 123;
MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

// Receiver (Rank 1)
int received_data;
MPI_Recv(&received_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

## Non-blocking Point-to-Point Communication

**Non-blocking Send**
int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);

**Non-blocking Receive**
int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);

**Wait for Completion**
int MPI_Wait(MPI_Request *request, MPI_Status *status);

**Example: Non-blocking Communication**
MPI_Request request;
MPI_Isend(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
// Do other work here
MPI_Wait(&request, MPI_STATUS_IGNORE);

## Collective Communication

**Broadcast**
int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);

- Broadcasts a message from the process with rank root to all other processes in the communicator.

**Example: Broadcast**
int data = 123;
MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);

**Scatter**
int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, int recvcount, MPI_Datatype recvtype,
                int root, MPI_Comm comm);

- Distributes distinct messages from the root process to all processes.

**Gather**
int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
               void *recvbuf, int recvcount, MPI_Datatype recvtype,
               int root, MPI_Comm comm);

- Gathers messages from all processes to the root process.

**Example: Scatter and Gather**
int send_data[4] = {1, 2, 3, 4}; // Assume 4 processes
int recv_data;
MPI_Scatter(send_data, 1, MPI_INT, &recv_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

// Each process now has one element of send_data

int gather_data[4];
MPI_Gather(&recv_data, 1, MPI_INT, gather_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

// Root process now has gathered data from all processes

**Reduce**
int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
               MPI_Op op, int root, MPI_Comm comm);

- Combines values from all processes and returns the result to the root process.

**Example: Reduce**
int local_data = 5;
int global_sum;
MPI_Reduce(&local_data, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

**Allreduce**
int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
                  MPI_Op op, MPI_Comm comm);

- Combines values from all processes and distributes the result to all processes.

**Example: Allreduce**
int local_data = 5;
int global_sum;
MPI_Allreduce(&local_data, &global_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

**Barrier**
int MPI_Barrier(MPI_Comm comm);

- Blocks until all processes in the communicator have reached the barrier.

**Example: Barrier**
MPI_Barrier(MPI_COMM_WORLD);

## Common Data Types and Operations

**Data Types**
- MPI_INT
- MPI_FLOAT
- MPI_DOUBLE
- MPI_CHAR
- MPI_LONG

**Operations**
- MPI_SUM: Sum
- MPI_PROD: Product
- MPI_MAX: Maximum
- MPI_MIN: Minimum

## Example: Complete Program

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int number;
    if (world_rank == 0) {
        number = -1;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (world_rank == 1) {
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number);
    }

    MPI_Finalize();
    return 0;
}
