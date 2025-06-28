import java.util.*;

public class SchedulerPolicy {
    // Task queue to hold pending tasks
    private PriorityQueue<Task> taskQueue;
    
    // Current tasks being processed
    private Task currentCpuTask;
    private Task currentIoTask;
    
    public SchedulerPolicy() {
        // Initialize the task queue with a comparator that prioritizes:
        // 1. Higher priority tasks
        // 2. Tasks with earlier deadlines
        taskQueue = new PriorityQueue<>((t1, t2) -> {
            if (t1.priority != t2.priority) {
                return t1.priority == Task.Priority.HIGH ? -1 : 1;
            }
            return Integer.compare(t1.deadline, t2.deadline);
        });
    }
    
    public Action policy(List<Event> events, int currentCpuTaskId, int currentIoTaskId) {
        // Update current tasks based on input parameters
        currentCpuTask = findTaskById(currentCpuTaskId);
        currentIoTask = findTaskById(currentIoTaskId);
        
        // Process all events
        for (Event event : events) {
            switch (event.type) {
                case TASK_ARRIVAL:
                    taskQueue.add(event.task);
                    break;
                case TASK_FINISH:
                    // Task completed, remove from system
                    if (currentCpuTask != null && currentCpuTask.taskId == event.task.taskId) {
                        currentCpuTask = null;
                    }
                    break;
                case IO_REQUEST:
                    // Task requests IO, needs to yield CPU
                    if (currentCpuTask != null && currentCpuTask.taskId == event.task.taskId) {
                        currentCpuTask = null;
                    }
                    break;
                case IO_END:
                    // IO completed, task can resume CPU work
                    if (currentIoTask != null && currentIoTask.taskId == event.task.taskId) {
                        currentIoTask = null;
                        taskQueue.add(event.task);
                    }
                    break;
                case TIMER:
                    // Timer interrupt, opportunity to reschedule
                    break;
            }
        }
        
        // Schedule IO task if IO is idle and there's a task waiting for IO
        if (currentIoTask == null) {
            // Find a task that needs IO (simplified logic)
            // In a real implementation, you'd track which tasks are waiting for IO
        }
        
        // Schedule CPU task if CPU is idle
        if (currentCpuTask == null && !taskQueue.isEmpty()) {
            currentCpuTask = taskQueue.poll();
        }
        
        // Prepare action
        Action action = new Action();
        action.cpuTask = currentCpuTask != null ? currentCpuTask.taskId : 0;
        action.ioTask = currentIoTask != null ? currentIoTask.taskId : 0;
        
        return action;
    }
    
    private Task findTaskById(int taskId) {
        if (taskId == 0) return null;
        
        // In a real implementation, you'd have a way to look up tasks by ID
        // This is simplified for demonstration
        return null;
    }
    
    // Define the required classes to match the C++ interface
    public static class Event {
        public enum Type {
            TIMER, TASK_ARRIVAL, TASK_FINISH, IO_REQUEST, IO_END
        }
        
        public Type type;
        public int time;
        public Task task;
    }
    
    public static class Task {
        public enum Priority {
            HIGH, LOW
        }
        
        public int arrivalTime;
        public int deadline;
        public Priority priority;
        public int taskId;
    }
    
    public static class Action {
        public int cpuTask;
        public int ioTask;
    }
}