.PHONY: clean

clean:
	cd ./application && make clean
	cd ./asyncLogger && make clean
	cd ./configuration && make clean
	cd ./dns && make clean
	cd ./errorThread && make clean
	cd ./logger && make clean
	cd ./loggingConfigurator && make clean
	cd ./serverApplication && make clean
	cd ./thread && make clean
	cd ./reactor && make clean
