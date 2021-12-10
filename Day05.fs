open System.IO

let CountMultiple x = Array.countBy id x |> Array.filter(fun (_,c)-> c>1) |> Array.length

let GenerateDiag (line:int[]) =
    let xs = [|line.[0] .. line.[2]|]
    let dy = if line.[3]-line.[1] > 0 then 1 else -1
    Array.map(fun x -> (x, line.[1] + dy *(x-line.[0]))) xs

[<EntryPoint>]
let main argv =
    let arrows = File.ReadAllLines(argv.[0]) |> Array.map(fun (s:string) -> s.Replace(',',' ').Replace(" -> ", " ").Split(' ') |> Array.map System.Int32.Parse)
    let (s,d) = Array.partition(fun (x:int[]) -> x.[0]=x.[2] || x.[1]=x.[3]) arrows
    let (h,v) = Array.partition(fun (x:int[]) -> x.[0]=x.[2]) s
    let p = Array.map(fun (x:int[]) -> ([|min x.[1] x.[3] .. max x.[1] x.[3]|] |> Array.map(fun a -> (x.[0],a)))) h |> Array.concat
    let points = Array.map(fun (x:int[]) -> ([|min x.[0] x.[2] .. max x.[0] x.[2]|] |> Array.map(fun a -> (a, x.[1])))) v |> Array.concat |> Array.append p
    CountMultiple points |> printfn "Part 1 %A" 
    let diags = Array.map(fun (x:int[]) -> if x.[0] < x.[2] then x else [|x.[2]; x.[3]; x.[0]; x.[1]|]) d
    CountMultiple (Array.map GenerateDiag diags |> Array.concat |> Array.append points) |> printfn "Part 2 %A"
    0
