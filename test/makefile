.PHONY:ALL
ALL:  cli1 cli2 cli3 cli4 cli5 cli6

cli1:test_client1.cpp
	g++ -o $@ $^ -std=c++17 ../Http/libmy_httpserver_lib.a
cli2:test_client2.cpp
	g++ -o $@ $^ -std=c++17 ../Http/libmy_httpserver_lib.a
cli3:test_client3.cpp
	g++ -o $@ $^ -std=c++17 ../Http/libmy_httpserver_lib.a
cli4:test_client4.cpp
	g++ -o $@ $^ -std=c++17 ../Http/libmy_httpserver_lib.a
cli5:test_client5.cpp
	g++ -o $@ $^ -std=c++17 ../Http/libmy_httpserver_lib.a
cli6:test_client6.cpp
	g++ -o $@ $^ -std=c++17 ../Http/libmy_httpserver_lib.a
.PHONY:clean
clean:
	rm -f cli svr test_util cli2 cli3 cli4 cli5