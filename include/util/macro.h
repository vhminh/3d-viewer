#define DELETE_COPY_CTOR(class_name)

#define NO_COPY(Class)                                                         \
	Class(const Class&) = delete;                                              \
	Class& operator=(const Class&) = delete

#define MOVE(Class)                                                            \
	Class(Class&&);                                                            \
	Class& operator=(Class&&)
