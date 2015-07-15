// 1. Done

package main

import (
	"fmt"
	s "strings"
)

func main() {
	var count int64
	for i := int64(0); i <= 255; i += 2 {
		fmt.Printf(s.Replace(fmt.Sprintf("%4d\t%16b\t%4d\t%16b\n", count, count, i, i), " ", "0", -1))
		count++
	}
	for hiByte := int64(0); hiByte < 2048; hiByte += 256 {
		for i := int64(0 + hiByte); i <= (512 + hiByte); i += 2 {
			fmt.Printf(s.Replace(fmt.Sprintf("%4d\t%16b\t%4d\t%16b\n", count, count, i, i), " ", "0", -1))
			count++
		}
	}
	for i := int64(2048 - 256); i <= 2048; i += 2 {
		fmt.Printf(s.Replace(fmt.Sprintf("%4d\t%16b\t%4d\t%16b\n", count, count, i, i), " ", "0", -1))
		count++
	}

}
