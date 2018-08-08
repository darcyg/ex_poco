.PHONY: clean

clean:
	cd ./application && make clean
	cd ./config && make clean
	cd ./dns && make clean
	cd ./serverApplication && make clean
	cd ./thread && make clean
