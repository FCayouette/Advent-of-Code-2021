open System.IO

let ParseData (x:string) =
    let data = x.Split ' '
    data.[0].[0], System.Int32.Parse data.[1]

[<EntryPoint>]
let main argv =
    let data = File.ReadAllLines argv.[0] |> Seq.map ParseData
    let h,d,d2 = Seq.fold(fun (h,a,d) (inst, v) -> match inst with | 'u'->(h,a-v,d) | 'd'->(h,a+v,d) | _->(h+v, a, d+a*v)) (0,0,0) data
    printfn "Part 1: %A \nPart 2: %A" (h*d) (h*d2) 
    0 // return an integer exit code
