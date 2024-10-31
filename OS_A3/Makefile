SUBDIRS = Q0 Q1 Q2 Q3 Q4 Q5-6 Q7 Q8-9 Q10

.PHONY: all clean $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done