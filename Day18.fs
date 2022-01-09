open System.IO
 
let ShouldExplode n =
    let mutable depth = 0
    let mutable index = 0
    Array.iteri(fun i c -> if c=2 then depth<-(depth-1) 
                           elif c=1 then 
                            depth <- depth+1
                            if depth = 5 && index = 0 then index<-i) n
    index

let ExplodeAt (n:int[]) i =
    let mutable (left, right) = (n.[i+1] - 10, n.[i+3] - 10)
    Array.iteri(fun j x -> if j > i+4 && x > 9 then 
                            n.[j] <- n.[j]+right  
                            right <- 0) n
    let li = Array.tryFindIndexBack(fun x -> x > 9) n.[0..i-1]
    if None <> li then  n.[li.Value] <- n.[li.Value] + left
    Array.concat [n.[0..i-1]; [|10|]; n.[i+5..n.Length-1]]

let SplitAt (n:int[]) i =
    let v = n.[i] - 10
    let left = v/2
    Array.concat [n.[0..i-1]; [|1;left+10;3;v-left+10;2|]; n.[i+1..n.Length-1]]

let ReduceSum x y = 
    let mutable (num, go) = (Array.concat [[|1|]; x; [|3|]; y; [|2|]], true)
    while go do
        let explIndex = ShouldExplode num
        if explIndex > 0 then num <- ExplodeAt num explIndex else
        let split = Array.tryFindIndex(fun c -> c>=20) num
        if split.IsSome then
            let i = split.Value
            num <- SplitAt num i
        else go <- false
    num

let rec Magnitude i (num:int[]) =
    if num.[i] > 9 then (num.[i]-10, i+1) else
    let (left, comma) = Magnitude (i+1) num
    let (right, close) = Magnitude (comma + 1) num
    (3*left + 2*right, close+1)

[<EntryPoint>]
let main argv =
    let snailNumbers = File.ReadAllLines(argv.[0]) |> Array.map(fun (s:string) -> s.ToCharArray() |> Array.map(fun c -> match c with | '['-> 1 | ']' ->2 | ',' -> 3 | _ -> (int)c - (int)'0' + 10))
    Array.fold(fun t x -> ReduceSum t x) snailNumbers.[0] snailNumbers.[1..snailNumbers.Length-1] |> Magnitude 0 |> fst |> printfn "Part 1: %i"
    Array.fold(fun m i -> max m (Array.map(fun x -> ReduceSum snailNumbers.[i] x |> Magnitude 0 |> fst) snailNumbers |> Array.max)) 0 [|0..snailNumbers.Length-1|] |> printfn "Part 2: %i"
    0
