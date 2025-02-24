server:
	g++ dynamic_main.cpp MysqlOperator.cpp BaseDetector.cpp PhoneDetector.cpp\
	 DetectorManager.cpp BaseMasking.cpp StarMasking.cpp AddressMasking.cpp\
	 SensitiveFieldsManager.cpp\
	 -o server -levent -lmysqlclient -std=c++11 -lsqlparser

.PHONY:clean
clean:
	rm server