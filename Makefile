.PHONY: thesis clean iso rays

thesis:
	make -C thesis

rays:
	make -C code/data-processing

clean:
	find . -regex "\(.*__pycache__.*\|*.py[co]\)" -exec rm -rf {} +
	rm -rf cd.iso
	make clean -C thesis
	make clean -C code/data-processing

iso: thesis clean
	mkisofs -o cd.iso -posix-L -allow-lowercase -iso-level 3 -m '.g*' -m '.*' -R -relaxed-filenames -joliet-long -l -V "Jasek - Master thesis" -no-bak cd
