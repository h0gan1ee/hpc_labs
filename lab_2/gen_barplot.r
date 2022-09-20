bench = matrix(c(589, 105434, 1126105,
                 614, 26438, 330504,
                 564, 3441, 48524),
               ncol = 3)

colors = gray.colors(3, start = 0.5)

colnames(bench) = c("User", "OpenBLAS", "Apple's BLAS")
rownames(bench) = c(expression("3, 4, 5"),
                    expression("100, 400, 200"),
                    expression("1000, 1000, 100"))

par(family = "CMU Serif", mar = c(4, 7, 4, 4))
p = barplot(height = bench, 
        main = "DGEMM Benchmark",
        ylab = expression(italic(time)/μs),
        log = "y",
        ylim = c(1e2, 1e6),
        col = colors,
        beside = TRUE, 
        legend.text = FALSE)
legend("topright",
       title = expression(list(italic(m), italic(n), italic(k))),
       legend = rownames(bench),
       x.intersp = 0.5,
       fill = colors)
text(x = p, y = bench*0.75, cex = 0.7, labels = bench)