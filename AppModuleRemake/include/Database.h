#ifndef DATABASE_H
#define DATABASE_H

#include "../src/database/abstractions/Data.h"
#include "../src/database/abstractions/DataAdapter.h"
#include "../src/database/abstractions/DataModificator.h"
#include "../src/database/abstractions/DataRemover.h"
#include "../src/database/abstractions/DataUploader.h"
#include "../src/database/abstractions/DatabaseInitializer.h"
#include "../src/database/abstractions/DatabaseUtil.h"
#include "../src/database/postgresql/Postgresql.h"
#include "../src/database/postgresql/workpiece/upload/UserWorkpiece.h"
#include "../src/database/postgresql/workpiece/upload/TestWorkpiece.h"
#include "../src/database/postgresql/workpiece/upload/QuestWorkpiece.h"
#include "../src/database/postgresql/workpiece/upload/AnswerWorkpiece.h"
#include "../src/database/postgresql/workpiece/upload/MarkWorkpiece.h"
#include "../src/database/postgresql/workpiece/upload/CourseWorkpiece.h"

#endif