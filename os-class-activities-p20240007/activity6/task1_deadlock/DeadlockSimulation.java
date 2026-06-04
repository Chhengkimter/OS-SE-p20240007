import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicBoolean;

class Account {
    String name;
    int balance;
    Semaphore lock = new Semaphore(1);

    Account(String name, int balance) {
        this.name = name;
        this.balance = balance;
    }
}

class Transfer {
    static void transfer(Account from, Account to, int amount, AtomicBoolean transferDone) {
        try {
            System.out.println(Thread.currentThread().getName()
                    + " trying to lock FROM " + from.name);
            from.lock.acquire();
            System.out.println(Thread.currentThread().getName()
                    + " locked FROM " + from.name + " | now waiting for " + to.name);

            Thread.sleep(200);

            System.out.println(Thread.currentThread().getName()
                    + " trying to lock TO " + to.name + " (WAITING...)");
            to.lock.acquire();
            System.out.println(Thread.currentThread().getName()
                    + " locked TO " + to.name);

            from.balance -= amount;
            to.balance += amount;
            transferDone.set(true);

            System.out.println(Thread.currentThread().getName()
                    + " transfer completed: " + amount + " from " + from.name + " to " + to.name);

            to.lock.release();
            from.lock.release();
        } catch (InterruptedException e) {
            System.out.println(Thread.currentThread().getName() + " was interrupted.");
        }
    }
}

public class DeadlockSimulation {
    public static void main(String[] args) throws InterruptedException {
        Account accountA = new Account("Account-A", 1000);
        Account accountB = new Account("Account-B", 1000);

        System.out.println("=== Bank Transfer Deadlock Simulation ===");
        System.out.println("Starting balances -> Account-A: " + accountA.balance + " | Account-B: " + accountB.balance);
        System.out.println("Starting total: " + (accountA.balance + accountB.balance));
        System.out.println();

        AtomicBoolean t1Done = new AtomicBoolean(false);
        AtomicBoolean t2Done = new AtomicBoolean(false);

        Thread t1 = new Thread(() ->
                Transfer.transfer(accountA, accountB, 100, t1Done), "Thread-1");

        Thread t2 = new Thread(() ->
                Transfer.transfer(accountB, accountA, 200, t2Done), "Thread-2");

        t1.start();
        t2.start();

        // Watchdog: wait 3 seconds, then check if stuck
        Thread watchdog = new Thread(() -> {
            try {
                Thread.sleep(3000);
                if (!t1Done.get() || !t2Done.get()) {
                    System.out.println();
                    System.out.println("*** Deadlock detected: transactions are stuck ***");
                    System.out.println("Thread-1 is waiting for Account-B");
                    System.out.println("Thread-2 is waiting for Account-A");
                    System.out.println("Neither thread can proceed. Program will exit.");
                    System.out.println();
                    System.out.println("Final balances (unchanged) -> Account-A: "
                            + accountA.balance + " | Account-B: " + accountB.balance);
                    t1.interrupt();
                    t2.interrupt();
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        watchdog.setDaemon(true);
        watchdog.start();

        t1.join();
        t2.join();
    }
}
