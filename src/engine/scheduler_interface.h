// The Firmament project
// Copyright (c) 2011-2012 Malte Schwarzkopf <malte.schwarzkopf@cl.cam.ac.uk>
//
// The scheduler interface assumed by the engine.

#ifndef FIRMAMENT_ENGINE_SCHEDULER_INTERFACE_H
#define FIRMAMENT_ENGINE_SCHEDULER_INTERFACE_H

#include <set>

#include "base/types.h"
#include "base/job_desc.pb.h"
#include "misc/printable_interface.h"

namespace firmament {
namespace scheduler {

class SchedulerInterface : public PrintableInterface {
 public:
  // Constructor.
  SchedulerInterface(shared_ptr<JobMap_t> job_map,
                     shared_ptr<ResourceMap_t> resource_map)
      : job_map_(job_map), resource_map_(resource_map) {}
  // Adds a job to the set of active jobs that are considered for scheduling.
  // TODO(malte): Determine if we actually need this, given the reactive design
  // of the scheduler.
  //void AddJob(shared_ptr<JobDescriptor> job_desc);
  // Finds runnable tasks for the job in the argument and adds them to the
  // global runnable set.
  virtual const set<shared_ptr<TaskDescriptor> >& RunnableTasksForJob(
      shared_ptr<JobDescriptor> job_desc) = 0;
  // Schedules all runnable tasks in a job.
  // Returns the number of tasks scheduled.
  virtual uint64_t ScheduleJob(shared_ptr<JobDescriptor> job_desc) = 0;
  // Runs a scheduling iteration for all active jobs.
  // TODO(malte): Determine if the need this, given the reactive design of the
  // scheduler.
  //void ScheduleAllJobs();
 protected:
  // Binds a task to a resource, i.e. effects a scheduling assignment. This will
  // modify various bits of meta-data tracking assignments. It will then
  // delegate the actual execution of the task binary to the appropriate local
  // execution handler.
  virtual void BindTaskToResource(shared_ptr<TaskDescriptor> task_desc,
                                  ResourceDescriptor* res_desc) = 0;
  // Finds a resource for a runnable task. This is the core placement logic.
  // Returns the resource ID of the resource chosen in the second argument, or
  // NULL if no resource could be found.
  virtual const ResourceID_t* FindResourceForTask(
      shared_ptr<TaskDescriptor> task_desc) = 0;
  // Pointers to the associated coordinator's job and resource maps
  shared_ptr<JobMap_t> job_map_;
  shared_ptr<ResourceMap_t> resource_map_;
};

}  // namespace scheduler
}  // namespace firmament

#endif  // FIRMAMENT_ENGINE_SCHEDULER_INTERFACE_H