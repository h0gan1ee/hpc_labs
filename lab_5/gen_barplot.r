bench <- matrix(c(
    387, 1320, 1747, 236609, 649523,
    5901, 6385, 4908, 55715, 135693,
    421, 386, 3314, 11663, 28569
  ),
  ncol = 5
)

colors <- gray.colors(3, start = 0.5)

colnames(bench) <- c("OpenMP", "User", "Apple's BLAS")
rownames(bench) <- c(
        expression("3, 4, 5"),
        expression("100, 400, 200"),
        expression("1000, 1000, 100")
)

par(family = "CMU Serif", mar = c(4, 7, 4, 4))
p <- plot(bench, type = "b")
legend(
        "topright",
        title = expression(list(italic(m), italic(n), italic(k))),
        legend = rownames(bench),
        x.intersp = 0.5,
        fill = colors
)
text(x = p, y = bench-1000, cex = 0.7, labels = bench)
