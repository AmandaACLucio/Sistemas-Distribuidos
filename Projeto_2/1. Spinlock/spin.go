package main

import (
	"fmt"
	"math/rand"
	"sync"
	"sync/atomic"
	"time"
)

const n int = 100000000

func main() {
	var numerosAleatorios [n]int
	var Threads1 [9][10]int64

	gerarNumerosAleatorios(&numerosAleatorios)
	var nThreads = [9]int{1, 2, 4, 8, 16, 32, 64, 128, 256}
	for o := 0; o < 10; o++ {
		for index, element := range nThreads {

			start := time.Now()
			goroutines(&numerosAleatorios, element) // vetor de números aleatórios, quantidade de threads
			duration := time.Since(start)
			fmt.Println("Tempo de execução: ", duration.Microseconds(), "Microsegundos, com: ", element, " Threads")
			Threads1[index][o] = duration.Microseconds()
		}
	}
	for i := range Threads1 {
		fmt.Print(" Threads ", nThreads[i])
		for _, n := range Threads1[i] {
			fmt.Print(" - ", n)
		}
		fmt.Println(" ")
	}
}

//--------------------Gera números aleatórios
func gerarNumerosAleatorios(numerosAleatorios *[n]int) {
	min := -100
	max := 100
	for i := 0; i < n; i++ {
		numerosAleatorios[i] += (rand.Intn(max-min) + min)
		//fmt.Println("int: ", numerosAleatorios[i])
	}
}

//--------------------Cria threads
func goroutines(numerosAleatorios *[n]int, k int) {
	var tamanho int = len(numerosAleatorios) / k
	var valorFinal int
	//fmt.Println("tamanho = ", tamanho)
	var wg sync.WaitGroup
	var mu sync.Mutex
	//
	mylock := Mylock{
		held: 0,
	}
	//
	for i := 0; i < k; i++ {
		//fmt.Println("thread ", i, " iniciando")
		wg.Add(1)
		go soma(numerosAleatorios, tamanho, i, &wg, &mu, &valorFinal, mylock)
	}

	wg.Wait()
	fmt.Println("Valor total do vetor: ", valorFinal)
}

//--------------------Função onde as threads irão realizar a soma
func soma(numerosAleatorios *[n]int, tamanho int, i int, wg *sync.WaitGroup, mu *sync.Mutex, valorFinal *int, mylock Mylock) {
	var total int

	for o := i * tamanho; o < tamanho*(i+1); o++ {
		//fmt.Println("thread : ", i, " contando")
		total += numerosAleatorios[o]
	}

	//fmt.Println("thread : ", i, " somou : ", total)
	mylock.acquire()
	*valorFinal += total
	mylock.release()
	defer wg.Done()
}

type Mylock struct {
	held uint32
}

func (l Mylock) acquire() {
	for !atomic.CompareAndSwapUint32(&l.held, 0, 1) {
	}
}

func (l Mylock) release() {
	l.held = 0
}
