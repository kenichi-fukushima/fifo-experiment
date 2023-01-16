package main

import "bufio"
import "fmt"
import "os"
import "time"

const (
	REQ_FIFO_PATH = "/home/keni/Projects/fifo-experiment/fifo_req"
	RES_FIFO_PATH = "/home/keni/Projects/fifo-experiment/fifo_res"
)

func TimeString() string {
	return time.Now().Format("15:04:05.000")
}

func main() {
	f_req, _ := os.OpenFile(REQ_FIFO_PATH, os.O_WRONLY, 0644)
	f_res, _ := os.OpenFile(RES_FIFO_PATH, os.O_RDONLY, 0644)
	res_scanner := bufio.NewScanner(f_res)

	closeCh := make(chan struct{})

	go func() {
	L:
		for {
			select {
			case <-closeCh:
				f_req.Close()
				break L
			default:
				f_req.WriteString("[" + TimeString() + "] abc\n")
				time.Sleep(time.Millisecond * 500)
			}
		}
	}()

	go func() {
		for res_scanner.Scan() {
			fmt.Println("[" + TimeString() + "] Returned to caller: " + res_scanner.Text())
		}
	}()

	time.Sleep(time.Second * 10)
	close(closeCh)
}
