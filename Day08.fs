open System.IO

let Sort (x:string[]) =
    Array.map(fun (s:string) -> s.ToCharArray() |> Array.sort |> System.String.Concat) x

let IsSubset (super:string) (s:string) =
    let split = super.ToCharArray()
    Array.forall(fun c -> Array.contains c split) (s.ToCharArray())
    
let Decode (x:string[]) =
    let result = [|for _ in 0..9 -> ""|]
    let bySize = Array.sortBy(fun (x:string) -> x.Length) x
    result.[1] <- bySize.[0]
    result.[7] <- bySize.[1]
    result.[4] <- bySize.[2]
    result.[8] <- bySize.[9]
    Array.sub bySize 6 3 |> Array.iter(fun (x:string) -> if IsSubset x result.[4] then result.[9] <- x elif IsSubset x result.[1] then result.[0] <- x else result.[6] <- x)
    Array.sub bySize 3 3 |> Array.iter(fun (x:string) -> if IsSubset x result.[1] then result.[3] <- x elif IsSubset result.[6] x then result.[5] <- x else result.[2] <- x)
    result

let ToNum (keys:string[]) (num:string[]) =
    Array.fold(fun total (x:string) -> total * 10 + (Array.findIndex(fun k -> k = x) keys)) 0 num

[<EntryPoint>]
let main argv =
    let data = File.ReadAllLines(argv.[0]) |> Array.map(fun (x:string)-> x.Replace(" | ", "|").Split('|')) |> Array.map(fun x -> (x.[0].Split(' '), x.[1].Split(' ')))
    Array.fold(fun t (_, x:string[])-> t + (Array.filter(fun (y:string) -> y.Length <= 4 || y.Length = 7) x |> Array.length)) 0 data |> printfn "Part 1: %A"
    Array.map(fun ((a:string[]), (b:string[])) -> (Decode (Sort a)), Sort b) data |> Array.fold(fun t (a,b)-> t + ToNum a b) 0 |> printfn "Part 2 %A" 
    0
