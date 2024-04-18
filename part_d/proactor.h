typedef void (*event_handler)(int fd);

typedef struct {
    int fd;
    event_handler handler;
} ProactorTask;

void execute_proactor(ProactorTask *task);
